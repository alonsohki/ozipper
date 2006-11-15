#include "exception.h"
#include "languagetpl.h"
#include "shipfactory.h"
#include "http.h"
#include <fstream>
#include <sstream>
#include <libxml/parser.h>
#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <sys/stat.h>

class Ozipper
{
public:
  static int main(int argc, char *argv[], const char *env[])
  {
    chdir("../..");

    try
    {
      std::ostringstream xml;

      Http http(env);

      printf("Content-type: text/html; charset=UTF-8\r\n\r\n");

      if (http.POST("report") == "" || http.POST("lang") == "")
      {
        printf("1Empty report or lang variables");
      }
      else if (http.POST("template") == "")
      {
        printf("2Unable to find template");
      }
      else
      {
        LanguageTemplate tpl(http.POST("lang"));
        const ReportData &rdata = tpl.Parse(http.POST("report"));
        
        CreateXML(rdata, xml, http);
        xmlDoc * res = XSLParse(xml.str().c_str(), "standard");
        XSLDecorate(res, http.POST("template"));
      }
    }
    catch (Exception &e)
    {
      printf("1Exception caught at %s(%d): %s\n", e.GetFile(), e.GetLine(), e.GetErr());
//      fprintf(stderr, "Exception caught at %s(%d): %s\n", e.GetFile(), e.GetLine(), e.GetErr());
    }

    return 0;
  }

  static void XSLDecorate(xmlDoc *xml, const std::string& tpl)
  {
    std::string path = "templates/decoration/" + tpl + ".xsl";

    xsltStylesheet *xsl = xsltParseStylesheetFile((const xmlChar *)path.c_str());
    xmlDoc *res = xsltApplyStylesheet(xsl, xml, 0);

    printf("0");
    xsltSaveResultToFile(stdout, res, xsl);

    xsltFreeStylesheet(xsl);
    xmlFreeDoc(res);
    xmlFreeDoc(xml);
    xsltCleanupGlobals();
    xmlCleanupParser();
  }

  static xmlDoc * XSLParse(const char *xml_, const std::string& tpl)
  {
    std::string path = "templates/" + tpl + ".xsl";

    xmlDoc *xml = xmlParseDoc((const xmlChar *)xml_);
    xsltStylesheet *xsl = xsltParseStylesheetFile((const xmlChar *)path.c_str());
    xmlDoc *res = xsltApplyStylesheet(xsl, xml, 0);

    xsltFreeStylesheet(xsl);
    xmlFreeDoc(xml);

    return res;
  }

  static void CreateXML(const ReportData& res, std::ostringstream& xml, Http& http) throw(Exception)
  {
    std::ifstream translation;
    std::string path;
    char *tmp;
    struct stat state;
    
    /* Inicializamos el xml */
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    
    /* Escribimos datos extra persistentes */
    xml << "<xml>\n"
           "  <extra>\n"
           "    <nullresources>\n"
           "      <metal>0</metal>\n"
           "      <crystal>0</crystal>\n"
           "      <deuterium>0</deuterium>\n"
           "    </nullresources>\n"
     "  </extra>\n\n";
    
    /* Escribimos el node-set de traducción */
    if (http.POST("outputlang") != "")
    {
      path = "lang/" + http.POST("outputlang") + "/translation.xml";
    }
    else
    {
      path = "lang/" + http.POST("lang") + "/translation.xml";
    }
    
    stat(path.c_str(), &state);
    tmp = new char[state.st_size + 1];
    translation.open(path.c_str());
    if (!translation.is_open())
    {
      EXCEPTION("Unable to open translation.xml");
    }
    translation.read(tmp, state.st_size);
    tmp[state.st_size] = '\0';
    xml << tmp;
    delete[] tmp;

    /* Escribimos las opciones */
    xml << "  <options>\n";
    xml << "    <option id=\"individual-units\">";
    if (http.POST("individual") == "1")
    {
      xml << "true";
    }
    else
    {
      xml << "false";
    }
    xml << "</option>\n";
    xml << "  </options>\n\n";

    /* Escribimos el resultado de la batalla */
    xml << "  <battle>\n";
    xml << "    <rounds>" << res.rounds << "</rounds>\n";
    xml << "    <date>\n";
    xml << "      <day>" << res.date.tm_mday << "</day>\n";
    xml << "      <month>" << res.date.tm_mon << "</month>\n";
    xml << "      <hour>" << res.date.tm_hour << "</hour>\n";
    xml << "      <minute>" << res.date.tm_min << "</minute>\n";
    xml << "      <second>" << res.date.tm_sec << "</second>\n";
    xml << "    </date>\n";

    for (std::vector<Player *>::const_iterator i = res.players.begin(); i != res.players.end(); i++)
    {
      Player *player = *i;
      unsigned int metal = 0;
      unsigned int crystal = 0;
      unsigned int deuterium = 0;
      
      xml << "    <player>\n";
      xml << "      <name>" << player->GetName() << "</name>\n";
      if (http.POST("coords") == "1")
      {
        xml << "      <coords>" << player->GetCoords() << "</coords>\n";
      }
      else
      {
        xml << "      <coords>X:X:X</coords>\n";
      }
      xml << "      <role>" << player->GetRole() << "</role>\n";
      xml << "      <techs>\n";
      if (http.POST("techs") == "1")
      {
        xml << "        <weapons>" << player->GetWeapons() << "</weapons>\n";
        xml << "        <shields>" << player->GetShield() << "</shields>\n";
        xml << "        <armour>" << player->GetArmour() << "</armour>\n";
      }
      else
      {
        xml << "        <weapons>XXX</weapons>\n";
        xml << "        <shields>XXX</shields>\n";
        xml << "        <armour>XXX</armour>\n";
      }
      xml << "      </techs>\n";
      
      xml << "      <fleet>\n";
      if (player->GetShips(0).begin() != player->GetShips(0).end())
      {
        for (std::map<const std::string, Ship>::const_reverse_iterator s = ShipFactory::getInstance()->GetShips().rbegin();
             s != ShipFactory::getInstance()->GetShips().rend();
             s++)
        {
          try
          {
            unsigned int initial = player->GetShipCount((*s).first, 0);
            unsigned int lost;

            if (res.rounds > 0)
            {
              try
              {
                lost = initial - player->GetShipCount((*s).first, 1);
              }
              catch (Exception&)
              {
                lost = initial;
              }
            }
            else
            {
              lost = 0;
            }
    
            metal += lost * (*s).second.metal;
            crystal += lost * (*s).second.crystal;
            deuterium += lost * (*s).second.deuterium;
    
            xml << "        <ship>\n";
            xml << "          <name>" << (*s).second.name << "</name>\n";
            xml << "          <initial>" << initial << "</initial>\n";
            xml << "          <lost>" << lost << "</lost>\n";
            xml << "        </ship>\n";
          }
          catch (Exception&) { }
        }
      }
      xml << "      </fleet>\n";

      xml << "      <losses>\n";
      xml << "        <metal>" << metal << "</metal>\n";
      xml << "        <crystal>" << crystal << "</crystal>\n";
      xml << "        <deuterium>" << deuterium << "</deuterium>\n";
      xml << "      </losses>\n";
      xml << "    </player>\n\n";
    }

    xml << "    <result>\n";
    xml << "      <winner>" << res.winner << "</winner>\n";
    xml << "      <captures>\n";
    xml << "        <metal>" << res.captures.metal << "</metal>\n";
    xml << "        <crystal>" << res.captures.crystal << "</crystal>\n";
    xml << "        <deuterium>" << res.captures.deuterium << "</deuterium>\n";
    xml << "      </captures>\n";
    xml << "    </result>\n\n";

    xml << "    <debris>\n";
    xml << "      <metal>" << res.debris.metal << "</metal>\n";
    xml << "      <crystal>" << res.debris.crystal << "</crystal>\n";
    xml << "    </debris>\n";

    xml << "    <moon>\n";
    xml << "      <chance>" << res.moonchance << "</chance>\n";
    xml << "      <formed>" << (res.moon ? "true" : "false") << "</formed>\n";
    xml << "    </moon>\n";
    
    xml << "  </battle>\n";
    
    xml << "</xml>";
  }
};

int main(int argc, char *argv[], const char *env[])
{
  Ozipper::main(argc, argv, env);
}
