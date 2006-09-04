function getHTTPObject()
{
  var xmlhttp;
  /*@cc_on
  @if (@_jscript_version >= 5)
    try {
      xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
    } catch (e) {
      try {
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
      } catch (E) {
        xmlhttp = false;
      }
    }
  @else
  xmlhttp = false;
  @end @*/
  if (!xmlhttp && typeof XMLHttpRequest != 'undefined') {
    try {
      xmlhttp = new XMLHttpRequest();
    } catch (e) {
      xmlhttp = false;
    }
  }
  return xmlhttp;
}
var http = getHTTPObject();

function HTMLObj(id)
{
  return document.getElementById(id);
}

function ShowWait(state)
{
  var obj1 = HTMLObj('result');
  var obj2 = HTMLObj('busy');
  var obj3 = HTMLObj('submit');

  /* Ocultamos el textarea y mostramos el reloj de arena */
  obj1.style.display = (state ? 'none' : '');
  obj2.style.display = (state ? '' : 'none');
  if (state)
  {
    obj3.disabled = true;
  }
  else
  {
    obj3.disabled = false;
  }
}

var format = 'bb_dark'; /* Valor por defecto */
function getFormat()
{
  return format;
}

function checkError(errnum)
{
  if (errnum != 0)
  {
    alert(errors[errnum - 1]);
    return true;
  }
  return false;
}

function AJAXResponse()
{
  if (http.readyState == 4)
  {
//    if (!checkError(http.responseText.charAt(0)))
//    {
      var text = http.responseText.substring(1);
      if (HTMLObj('align').value == 'center')
      {
        HTMLObj('compactado').value = '[center]' + text + '[/center]';
      }
      else
      {
        HTMLObj('compactado').value = text;
      }

      if (HTMLObj('template').value == 'bb_dark')
      {
        HTMLObj('wysiwyg').className = 'wysiwyg_dark';
      }
      else
      {
        HTMLObj('wysiwyg').className = 'wysiwyg_light';
      }
      format = HTMLObj('template').value;
      compactadoChange();
//    }
    ShowWait(false);
  }
}

function AJAXRequest()
{
  var server = "/cgi-bin/ozipper.cgi";
  ShowWait(true);

  if (!http)
  {
    alert('Error en AJAX');
    return false;
  }

  /* Preparamos la cadena de parámetros */
  var poststr = "lang=" + HTMLObj('selectedlang').value;

  switch (HTMLObj('show').value)
  {
    case "coords":
      poststr += "&coords=1";
      break;
    case "techs":
      poststr += "&techs=1";
      break;
    case "all":
      poststr += "&coords=1&techs=1";
      break;
  }
  poststr += "&individual=" + (HTMLObj('individual').checked ? "1" : "0") +
             "&template=" + HTMLObj('template').value +
	     "&outputlang=" + HTMLObj('outputlang').value +
             "&report=" + encodeURIComponent(HTMLObj('informe').value + "\n");
  
  http.open("POST", server, true);
  http.onreadystatechange = AJAXResponse;
  http.setRequestHeader("Accept-Charset", "UTF-8");
  http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  http.setRequestHeader("Charset", "UTF-8");
  http.setRequestHeader("Content-length", poststr.length);
  http.setRequestHeader("Connection", "close");
  http.send(poststr);

  return false; /* Nos aseguramos de que el formulario no se envíe */
}
