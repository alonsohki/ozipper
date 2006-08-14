#include "exception.h"
#include "languagetpl.h"
#include "shipfactory.h"
#include "http.h"
#include <fstream>
#include <sstream>
#include <locale.h>

class Ozipper
{
public:
  static int main(int argc, char *argv[], const char *env[])
  {
    try
    {
      char tmp[512];
      std::string out;
      std::ifstream file;
      size_t bytes;
      std::ostringstream xml;

      setlocale(LC_ALL, "es_ES.UTF-8");

      file.open(argv[1]);
      do
      {
        bytes = file.readsome(tmp, 512);
        tmp[bytes] = '\0';
        out += tmp;
      } while (bytes);
      file.close();

      Http http(env);

      if (http.GET("lang") == "")
      {
	printf("1");
      }
      else
      {
        LanguageTemplate tpl(http.GET("lang"));
        CreateXML(tpl.Parse(out), xml, http);
	puts(xml.str().c_str());
      }
    }
    catch (Exception &e)
    {
      printf("1");
      fprintf(stderr, "Exception caught at %s(%d): %s\n", e.GetFile(), e.GetLine(), e.GetErr());
    }

    return 0;
  }

  static void CreateXML(const ReportData& res, std::ostringstream& xml, Http& http) throw(Exception)
  {
    std::ifstream translation;
    std::string path;
    char tmp[512];
    size_t bytes;
    
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
    path = "lang/" + http.GET("lang") + "/translation.xml";
    translation.open(path.c_str());
    if (!translation.good())
    {
      EXCEPTION("Unable to open translation.xml");
    }
    do
    {
      bytes = translation.readsome(tmp, 511);
      if (bytes > 0)
      {
	tmp[bytes] = '\0';
	xml << tmp;
      }
    } while (bytes > 0);
    translation.close();

    /* Escribimos las opciones */
    xml << "  <options>\n";
    xml << "    <option id=\"individual-units\">";
    if (http.GET("individual") == "1")
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
    strftime(tmp, 511, "%d de %B, a las %H:%M:%S", &res.date);
    xml << "    <date>" << tmp << "</date>\n";

    for (std::vector<Player *>::const_iterator i = res.players.begin(); i != res.players.end(); i++)
    {
      Player *player = *i;
      unsigned int metal = 0;
      unsigned int crystal = 0;
      unsigned int deuterium = 0;
      
      xml << "    <player>\n";
      xml << "      <name>" << player->GetName() << "</name>\n";
      if (http.GET("coords") == "1")
      {
        xml << "      <coords>" << player->GetCoords() << "</coords>\n";
      }
      else
      {
        xml << "      <coords>X:X:X</coords>\n";
      }
      xml << "      <role>" << player->GetRole() << "</role>\n";
      xml << "      <techs>\n";
      if (http.GET("techs") == "1")
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
      for (std::map<const std::string, Ship>::const_reverse_iterator s = ShipFactory::getInstance()->GetShips().rbegin();
	   s != ShipFactory::getInstance()->GetShips().rend();
	   s++)
      {
	try
	{
	  unsigned int initial = player->GetShipCount((*s).first, 0);
	  unsigned int lost;

	  try
	  {
	    lost = initial - player->GetShipCount((*s).first, 1);
	  }
	  catch (Exception&)
	  {
	    lost = initial;
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
    
    xml << "  </battle>\n";
    
    xml << "</xml>";
  }
};

int main(int argc, char *argv[], const char *env[])
{
  Ozipper::main(argc, argv, env);
}
