/* Tipos de foros */
phpBB = function()
{
  this.getName      = function()     { return 'phpBB'; }
  this.getKeys      = function()     { return new Array('<span style="color: ', '<span style="font-size: ', '<a href="'); }
  this.getKeysEnd   = function()     { return new Array('">', 'px">', '">'); }
  this.getKeysClose = function()     { return new Array('</span>', '</span>', '</a>'); }
  this.searchColor  = function(text) { return text.search(/\[COLOR\=.*\].*\[\/COLOR\]/gi); }
  this.searchSize   = function(text) { return text.search(/\[SIZE\=.*\].*\[\/SIZE\]/gi); }
  this.searchUrl    = function(text) { return text.search(/\[URL\=.*\].*\[\/URL\]/gi); }
}

SMF = function()
{
  this.getName      = function()     { return 'SMF'; }
  this.getKeys      = function()     { return new Array('<span style="color: ', '<span style="font-size: ', '<a href="'); }
  this.getKeysEnd   = function()     { return new Array('">', '">', '">'); }
  this.getKeysClose = function()     { return new Array('</span>', '</span>', '</a>'); }
  this.searchColor  = function(text) { return text.search(/\[COLOR\=.*\].*\[\/COLOR\]/gi); }
  this.searchSize   = function(text) { return text.search(/\[SIZE\=.*pt\].*\[\/SIZE\]/gi); }
  this.searchUrl    = function(text) { return text.search(/\[URL\=.*\].*\[\/URL\]/gi); }
}

VBulletin = function()
{
  this.getName      = function()     { return 'VBulletin'; }
  this.getKeys      = function()     { return new Array('<font color="', '<font size="', '<a href="'); }
  this.getKeysEnd   = function()     { return new Array('">', '">', '">'); }
  this.getKeysClose = function()     { return new Array('</font>', '</font>', '</a>'); }
  this.searchColor  = function(text) { return text.search(/\[COLOR\=.*\].*\[\/COLOR\]/gi); }
  this.searchSize   = function(text) { return text.search(/\[SIZE\=.*\].*\[\/SIZE\]/gi); }
  this.searchUrl    = function(text) { return text.search(/\[URL\=.*\].*\[\/URL\]/gi); }
}

/* Gestor de tipos de foros */
forumManager = function()
{
  this.forums = new Array();

  this.addForum = function(forum)
  {
    this.forums[this.forums.length] = forum;
  }

  this.getForum = function(name)
  {
    for (var i = 0; i < this.forums.length; i++)
    {
      if (this.forums[i].getName() == name)
      {
        return this.forums[i];
      }
    }

    return -1;
  }
}

/* Generamos los tipos de foros */
var fmanager = new forumManager();
fmanager.addForum(new phpBB());
fmanager.addForum(new SMF());
fmanager.addForum(new VBulletin());

/* Procesamos caracteres especiales para evitar ejecución html en el 'preview' */
function stripTags(str)
{
  var text = '';
  var specialchars = new htmlspecialchars();
  
  for (i = 0; i < str.length; i++)
  {
    text += specialchars.Transform(str.charAt(i));
  }

  return text;
/*    switch (tex.charAt(i))
    {
      case '<':
        text += '&lt;';
        break;
      case '>':
        text += '&gt;';
        break;
      case '&':
        text += '&amp;';
        break;
      case '"':
        text += '&quot;';
        break;
      case "'":
        text += '&#039;';
        break;
      case '\n':
        text += '<br />';
        break;
      case 'á':
        text += '&aacute;';
        break;
      case 'é':
        text += '&eacute;';
        break;
      case 'í':
        text += '&iacute;';
        break;
      case 'ó':
        text += '&oacute;';
        break;
      case 'ú':
        text += '&uacute;';
        break;
      case 'Á':
        text += '&Aacute;';
        break;
      case 'É':
        text += '&Eacute;';
        break;
      case 'Í':
        text += '&Iacute;';
        break;
      case 'Ó':
        text += '&Oacute;';
        break;
      case 'Ú':
        text += '&Uacute;';
        break;
      default:
        text += tex.charAt(i);
    }*/
}

function parseCode(text_, forum)
{
  var forum = fmanager.getForum(forum);
  var text = stripTags(text_);

  /* Reemplazamos negritas */
  text = text.replace(/\[B\]/gi, '<b>');
  text = text.replace(/\[\/B\]/gi, '</b>');

  /* Reemplazamos subrayados */
  text = text.replace(/\[U\]/gi, '<u>');
  text = text.replace(/\[\/U\]/gi, '</u>');

  /* Reemplazamos cursivas */
  text = text.replace(/\[I\]/gi, '<i>');
  text = text.replace(/\[\/I\]/gi, '</i>');

  /* Reemplazamos centrado */
  text = text.replace(/\[CENTER\]/gi, '<div align="center">');
  text = text.replace(/\[\/CENTER\]/gi, '</div>');

  /* Buscamos colores, tamaños, urls */
  var ftext = '';
  positions = new Array(0, 0, 0);
 
  keys = forum.getKeys();
  keysend = forum.getKeysEnd();
  lengths = new Array(7, 6, 5);
  
  while (true)
  {
    positions[0] = forum.searchColor(text);
    positions[1] = forum.searchSize(text);
    positions[2] = forum.searchUrl(text);


    /* Buscamos la primera coincidencia */
    var m = -1;
    
    for (i = 0; i < 3; i++)
    {
      if (positions[i] == -1)
      {
        continue;
      }

      if ((m == -1) || (positions[i] < positions[m]))
      {
        m = i;
      }
    }

    /* No se ha encontrado nada */
    if (m == -1)
    {
      ftext += text;
      break;
    }

    pos = positions[m];

    ftext += text.substring(0, pos); /* Copiamos lo que hubiese antes del tag */
    pos += lengths[m]; /* Saltamos el comienzo del tag */
    text = text.substring(pos);
    posend = text.search(']');
    ftext += keys[m];
    value = text.substring(0, posend); /* Extraemos el valor del tag */
    text = text.substring(posend + 1);
    ftext += value + keysend[m];
  }
  text = ftext;

  /* Interpretamos urls sin value */
  ftext = '';
  while (true)
  {
    pos = text.search(/\[URL\].*\[\/URL\]/gi);
    if (pos == -1)
    {
      ftext += text;
      break;
    }

    ftext += text.substr(0, pos); /* Copiamos lo que hubiese antes del tag */
    pos += 5; /* Saltamos [URL] */
    text = text.substring(pos);
    posend = text.search(/\[\/URL\]/gi); /* Buscamos el final del tag */
    url = text.substring(0, posend); /* Extraemos la URL */
    posend += 6; /* Saltamos [/URL]*/
    text = text.substring(posend);
    ftext += '<a href="' + url + '">' + url + '</a>';
  }
  text = ftext;

  /* Cerramos los tags anteriores */
  var keysClose = forum.getKeysClose();
  text = text.replace(/\[\/COLOR\]/gi, keysClose[0]);
  text = text.replace(/\[\/SIZE\]/gi, keysClose[1]);
  text = text.replace(/\[\/URL\]/gi, keysClose[2]);

  return text;
}
