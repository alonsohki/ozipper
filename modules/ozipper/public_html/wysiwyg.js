function stripTags(tex)
{
  var text = '';
  
  for (i = 0; i < tex.length; i++)
  {
    switch (tex.charAt(i))
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
    }
  }

  return text;
}

function parseCode(text_, forum)
{
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
  keys    = new Array('<span style="color: ', '<span style="font-size: ', '<a href="');
  if (forum == 'bb')
  {
    keysend = new Array('">', 'px">', '">');
  }
  else if (forum == 'smf')
  {
    keysend = new Array('">', '">', '">');
  }
  lengths = new Array(7, 6, 5);
  
  while (true)
  {
    positions[0] = text.search(/\[COLOR\=.*\].*\[\/COLOR\]/gi);
    if (forum == 'bb')
    {
      positions[1] = text.search(/\[SIZE\=.*\].*\[\/SIZE\]/gi);
    }
    else if (forum == 'smf')
    {
      positions[1] = text.search(/\[SIZE\=.*pt\].*\[\/SIZE\]/gi);
    }
    positions[2] = text.search(/\[URL\=.*\].*\[\/URL\]/gi);


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
  text = text.replace(/\[\/COLOR\]/gi, '</span>');
  text = text.replace(/\[\/SIZE\]/gi, '</span>');
  text = text.replace(/\[\/URL\]/gi, '</a>');

  return text;
}
