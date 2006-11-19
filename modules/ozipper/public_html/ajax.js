Ajax = function(server_, callback_)
{
  var server = server_;
  var callback = callback_;
  var xmlhttp = null;

  function getHTTPObject()
  {
    if (!xmlhttp)
    {
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
    }

    return xmlhttp;
  }

  function StateChange()
  {
    http = getHTTPObject();

    if (http.readyState == 4)
    {
      data = new Array(http.responseText, http.responseXML);
      callback(data);
    }
  }

  this.Request = function(poststr)
  {
    http = getHTTPObject();

    if (!http)
    {
      alert('Ajax error');
      return false;
    }

    http.open('POST', server, true);
    http.onreadystatechange = StateChange;
    http.setRequestHeader("Accept-Charset", "UTF-8");
    http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    http.setRequestHeader("Charset", "UTF-8");
    http.setRequestHeader("Content-length", poststr.length);
    http.setRequestHeader("Connection", "close");
    http.send(poststr);
  }
}
