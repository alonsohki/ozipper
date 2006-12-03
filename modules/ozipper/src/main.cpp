#include "exception.h"
#include "intl.h"
#include "languagetpl.h"
#include "shipfactory.h"
#include "http.h"
#include <fstream>
#include <sstream>
#include <iostream>
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
    xmlDoc * output = xmlNewDoc((const xmlChar *)"1.0");
    xmlNode * rootNode = xmlNewNode(0, (const xmlChar *)"xml");
    int xmlLength;
    xmlChar *xmlDocument;
    
    chdir("../..");
    xmlDocSetRootElement(output, rootNode);

    std::cout << "Content-type: text/xml; charset=UTF-8\r\n\r\n";

    try
    {
      Http http(env);

      Intl::InitLocale(http.POST("lang").c_str());

      if (http.POST("report") == "" || http.POST("lang") == "")
      {
        Error(rootNode, Intl::_(EMPTY_REPORT_OR_LANG_VARS));
      }

      else if (http.POST("template") == "")
      {
        Error(rootNode, Intl::_(UNABLE_TO_FIND_REQUIRED_TEMPLATE));
      }
      else if (http.POST("reportinfo") == "" && http.POST("distributeinfo") == "")
      {
        Error(rootNode, Intl::_(NO_INFORMATION_REQUIRED));
      }
      else
      {
        LanguageTemplate tpl(http.POST("lang"));
        const ReportData &rdata = tpl.Parse(http.POST("report"));
        
        if (http.POST("reportinfo") == "1")
        {
          std::ostringstream xml;
          
          CreateXML(rdata, xml, http);
          xmlDoc * tmpxml = XSLParse(xml.str().c_str(), "standard");
          xmlDoc * res = XSLDecorate(tmpxml, http.POST("template"));

          xmlAddChild(rootNode, xmlDocGetRootElement(res));

          xmlFreeDoc(tmpxml);
        }
        
        if (http.POST("distributeinfo") == "1")
        {
          std::ostringstream xml;

          CreateDistributeXML(rdata, xml);
          xmlDoc * res = xmlParseDoc((const xmlChar *)xml.str().c_str());

          xmlAddChild(rootNode, xmlDocGetRootElement(res));
        }
      }
    }
    catch (Exception &e)
    {
//      Error(rootNode, "Exception caught at %s(%d): [%d] %s\n", e.GetFile(), e.GetLine(), e.GetErrID(), e.GetErr());
      Error(rootNode, Intl::_((StrID)e.GetErrID()));
    }

    xmlDocDumpMemoryEnc(output, &xmlDocument, &xmlLength, "UTF-8");
    printf((const char *)xmlDocument);
    xmlFree(xmlDocument);

    Cleanup();

    return 0;
  }

  static void CreateDistributeXML(const ReportData& rdata, std::ostringstream& output)
  {
    unsigned int count[2] = { 0, 0 };

    output << "<distribute>\n";
    
    for (std::vector<Player *>::const_iterator i = rdata.players.begin(); i != rdata.players.end(); i++)
    {
      Player *player = *i;
      Costs losses;
      Costs value;

      if (player->GetRole() == "attacker")
      {
        count[0]++;
      }
      else if (player->GetRole() == "defender")
      {
        count[1]++;
      }
      losses = player->GetLosses();
      value = player->GetValue();

      output << "  <player>\n";
      output << "    <name>" << player->GetName() << "</name>\n";
      output << "    <role>" << player->GetRole() << "</role>\n";
      output << "    <losses>\n";
      output << "      <metal>" << losses.metal << "</metal>\n";
      output << "      <crystal>" << losses.crystal << "</crystal>\n";
      output << "      <deuterium>" << losses.deuterium << "</deuterium>\n";
      output << "    </losses>\n";
      output << "    <value>\n";
      output << "      <metal>" << value.metal << "</metal>\n";
      output << "      <crystal>" << value.crystal << "</crystal>\n";
      output << "      <deuterium>" << value.deuterium << "</deuterium>\n";
      output << "    </value>\n";
      output << "  </player>\n";
    }
  
    output << "  <count>\n";
    output << "    <attacker>" << count[0] << "</attacker>\n";
    output << "    <defender>" << count[1] << "</defender>\n";
    output << "  </count>\n";
    output << "  <debris>\n";
    output << "    <metal>" << rdata.debris.metal << "</metal>\n";
    output << "    <crystal>" << rdata.debris.crystal << "</crystal>\n";
    output << "  </debris>\n";
    output << "</distribute>\n";
  }

  static void Error(xmlNode *rootNode, const char *errstr, ...)
  {
    va_list vl;
    char buffer[256];
    xmlNode * errorNode = xmlNewNode(0, (const xmlChar *)"error");
    xmlNode * textNode;

    va_start(vl, errstr);
    vsnprintf(buffer, 256, errstr, vl);
    va_end(vl);

    textNode = xmlNewText((const xmlChar *)buffer);
    xmlAddChild(errorNode, textNode);

    xmlAddChild(rootNode, errorNode);
  }

  static void Cleanup()
  {
    xsltCleanupGlobals();
    xmlCleanupParser();
  }

  static xmlDoc * XSLDecorate(xmlDoc *xml, const std::string& tpl)
  {
    std::string path = "templates/decoration/" + tpl + ".xsl";

    xsltStylesheet *xsl = xsltParseStylesheetFile((const xmlChar *)path.c_str());
    xmlDoc *res = xsltApplyStylesheet(xsl, xml, 0);

    xsltFreeStylesheet(xsl);
    return res;
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
      EXCEPTION(COULDNT_OPEN_TRANSLATION, "Unable to open translation.xml");
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
      Costs losses;
      
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

            losses = player->GetLosses();
    
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
      xml << "        <metal>" << losses.metal << "</metal>\n";
      xml << "        <crystal>" << losses.crystal << "</crystal>\n";
      xml << "        <deuterium>" << losses.deuterium << "</deuterium>\n";
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
