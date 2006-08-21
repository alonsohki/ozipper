<?
  require_once('lang/main.php');
  $lang = getLang();
  require_once('lang/' . $lang);

  /* Almacenamos en la cookie el idioma seleccionado */
  $p = strpos($lang, '.php');
  if ($p)
  {
    $langt = substr($lang, 0, $p);
    setcookie('lang', $langt, time() + (86400 * 7), '/', 'ozipper.net');
  }

  function isl($l)
  {
    global $lang;

    if (
        ($lang == 'es.php' && $l == 'es') ||
	($lang == 'en.php' && $l == 'en')
       )
    {
      echo " selected=\"selected\"";
    }
  }

  header('Content-type: text/html; charset=UTF-8');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
    <meta name="description" content="<?=T('meta_description')?>" />
    <meta name="keywords" content="<?=T('meta_keywords')?>" />

    <title><?=T('OGame zipper')?></title>
    <link rel="stylesheet" type="text/css" href="style.css" />
    <script type="text/javascript"><!--
      var errors = new Array(<?=$langarr['ajax_errors_array']?>);
    // --></script>
    <script type="text/javascript" src="wysiwyg.js"></script>
    <script type="text/javascript" src="ajax.js"></script>
    <script type="text/javascript"><!--
      function HTMLObj(id)
      {
        return document.getElementById(id);
      }

      function start()
      {
        compactadoChange();
	ShowWait(false);
      }

      function compactadoChange()
      {
        var obj = HTMLObj('compactado');
        var obj2 = HTMLObj('wysiwyg');

	var format = getFormat();
	if (format == 'bb_dark' || format == 'bb_light')
	{
	  obj2.innerHTML = parseBBCode(obj.value);
	}
	else
	{
	  obj2.innerHTML = stripTags(obj.value);
	}
      }

      function bselect()
      {
        var obj = HTMLObj('compactado');
	obj.focus();
	obj.select();
      }
    // --></script>
  </head>
  <body onload="start();">
    <div id="header">
    <table id="main">
      <tr>
        <td id="banner"><img src="banner.png" alt="banner" /></td>
	<td id="langselect">
          <form action="" method="get" id="langchange">
            <table>
	      <tr>
                <td colspan="2" id="title"><span><?=T('Language')?></span></td>
              </tr>
              <tr>
                <td id="desc"><span><?=T('Select your language:')?></span></td>
                <td id="content"><select name="lang" onchange="HTMLObj('langchange').submit();">
	                           <option value="en"<?=isl('en')?>><?=T('English')?></option>
	                           <option value="es"<?=isl('es')?>><?=T('Spanish')?></option>
                                 </select>
	        </td>
              </tr>
            </table>
          </form>
        </td>
        <td id="options">
	  <a href="<?=T('foxgame_url')?>"><?=T('Improve your game with FoxGame')?></a><br />
	  <a href="contact.php?lang=<?=$langt?>"><?=T('Contact')?></a><br />
	</td>
      </tr>
    </table>
    </div>
    
    <div id="content">
    <form action="" method="post" accept-charset="UTF-8" onsubmit="return AJAXRequest();">
    <table>
      <tr>
        <td id="informetd">
	  <span><?=T('Enter your combat report:')?></span><br />
	  <textarea id="informe" rows="1" cols="1"></textarea>
	  <input type="hidden" id="selectedlang" value="<?=$langt?>" />
	</td>
	<td id="compactadotd">
	  <div id="result">
	    <span><?=T('Result:')?></span><br />
	    <textarea id="compactado" onkeyup="compactadoChange();" rows="1" cols="1"></textarea>
	  </div>
	  <img src="busy.gif" id="busy" alt="Please wait" />
	</td>
      </tr>
      <tr>
        <td id="informetd"><input type="submit" value="<?=T('Process report')?>" id="submit" />&nbsp;
	                   <input type="reset" value="<?=T('Reset')?>" onclick="HTMLObj('wysiwyg').innerHTML=''" /></td>
	<td id="compactadotd" align="center"><input type="button" onclick="bselect();" value="<?=T('Select text')?>" /></td>
      </tr>
    </table>
    </form>
    </div>

    <div id="options">
      <table>
      <tr><td>
      <table id="optstable">
        <tr>
	  <td id="title" colspan="2"><span><?=T('Options')?></span></td>
	</tr>
	<tr>
	  <td id="desc"><span><?=T('Format:')?></span></td>
	  <td id="content"><select id="template">
	                     <option value="bb_dark" selected="selected">phpBB <?=T('Dark')?></option>
			     <option value="bb_light">phpBB <?=T('Light')?></option>
			     <option value="plain"><?=T('Plain text')?></option>
			   </select>
	  </td>
	</tr>
	<tr>
	  <td id="desc"><span><?=T('Show:')?></span></td>
	  <td id="content"><select id="show">
	                     <option value="nothing" selected="selected"><?=T('Nothing')?></option>
		             <option value="coords"><?=T('Coordinates')?></option>
		             <option value="techs"><?=T('Technologies')?></option>
		             <option value="all"><?=T('All')?></option>
	                   </select>
	  </td>
	</tr>
	<tr>
	  <td id="desc"><span><?=T('Align:')?></span></td>
	  <td id="content"><select id="align">
	                     <option value="left" selected="selected"><?=T('Left')?></option>
			     <option value="center"><?=T('Center')?></option>
	                   </select>
	  </td>
	</tr>
        <tr>
          <td id="desc"><span><?=T('Output language:')?></span></td>
          <td id="content"><select id="outputlang">
			     <option value="en"<?=isl('en')?>><?=T('English')?></option>
			     <option value="es"<?=isl('es')?>><?=T('Spanish')?></option>
                           </select>
          </td>
        </tr>
	<tr>
	  <td id="desc"><span><?=T('Individual units:')?></span></td>
	  <td id="content"><input type="checkbox" id="individual" checked="checked" /></td>
	</tr>
      </table>
      </td><td id="google">
      <div>
<script type="text/javascript"><!--
google_ad_client = "pub-1709658272618910";
google_ad_width = 468;
google_ad_height = 60;
google_ad_format = "468x60_as";
google_ad_type = "text_image";
google_ad_channel ="";
google_color_border = "EBECF5";
google_color_bg = "F0F1F7";
google_color_link = "0000FF";
google_color_text = "000000";
google_color_url = "008000";
//--></script>
<script type="text/javascript"
  src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
  </script></div>
      </td></tr>
      </table>
    </div>

    <div id="preview">
      <span id="desc"><?=T('Preview:')?></span>
      <div id="wysiwyg" class="wysiwyg_dark"></div>
    </div>
  </body>
</html>
