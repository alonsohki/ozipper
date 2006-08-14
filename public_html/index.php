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
      echo " selected";
    }
  }

  header('Content-type: text/html; charset=UTF-8');
?>
<html>
  <head>
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
    <div style="position: absolute"><img src="banner.png" alt="banner" /></div>
    <div id="langselect">
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
	  </tr>
	</table>
      </form>
    </div>
    
    <div id="content">
    <form action="" method="post" accept-charset="UTF-8" onsubmit="return AJAXRequest();">
    <table>
      <tr>
        <td id="informetd">
	  <span><?=T('Enter your combat report:')?></span><br />
	  <textarea id="informe"></textarea>
	  <input type="hidden" id="selectedlang" value="<?=$langt?>" />
	</td>
	<td id="compactadotd">
	  <div id="result">
	    <span><?=T('Result:')?></span><br />
	    <textarea id="compactado" onkeyup="compactadoChange();"></textarea>
	  </div>
	  <img src="busy.gif" id="busy" />
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
        <tr>
	  <td id="title" colspan="2"><span><?=T('Options')?></span></td>
	</tr>
	<tr>
	  <td id="desc"><span><?=T('Format:')?></span></td>
	  <td id="content"><select id="template">
	                     <option value="bb_dark" selected>phpBB <?=T('Dark')?></option>
			     <option value="bb_light">phpBB <?=T('Light')?></option>
			     <option value="plain"><?=T('Plain text')?></option>
			   </select>
	</tr>
	<tr>
	  <td id="desc"><span><?=T('Show:')?></span></td>
	  <td id="content"><select id="show">
	                     <option value="nothing"><?=T('Nothing')?></option>
		             <option value="coords"><?=T('Coordinates')?></option>
		             <option value="techs"><?=T('Technologies')?></option>
		             <option value="all" selected><?=T('All')?></option>
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
	  <td id="content"><input type="checkbox" id="individual" checked /></td>
      </table>
    </div>

    <div id="preview">
      <span id="desc"><?=T('Preview:')?></span>
      <div id="wysiwyg" class="wysiwyg_dark"></div>
    </div>
  </body>
</html>
