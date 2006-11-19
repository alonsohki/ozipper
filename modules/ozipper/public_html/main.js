Main = function()
{
  var style;
  var ajax;

  function $(id)
  {
    return document.getElementById(id);
  }
  
  this.Start = function()
  {
    style = new Style($('template').value, $('background').value);
    style.Update();
    ajax = new Ajax('/cgi-bin/ozipper.cgi', function(data) { main.ReadyRequest(data); });

    style.Working(false);

    StartEvents();
  }

  function StartEvents()
  {
    $('compactado').onkeyup   =   style.Update;
    $('reset').onclick        =   style.Update;
    $('selecttext').onclick   =   function() { main.SelectText(); }
    $('lang').onchange        =   function() { main.SelectLanguage(); }
    $('mainform').onsubmit    =   function() { main.Request(); return false; }
    $('template').onchange    =   function() { main.UpdateStyle(); }
    $('background').onchange  =   function() { main.UpdateStyle(); }
  }

  this.UpdateStyle = function()
  {
    style.SetTemplate($('template').value);
    style.SetBackground($('background').value);
  }

  this.SelectText = function()
  {
    $('compactado').focus();
    $('compactado').select();
  }

  this.SelectLanguage = function()
  {
    $('langchange').submit();
  }

  function MakePoststr()
  {
    poststr =  'lang=' + $('selectedlang').value;
    poststr += '&reportinfo=1&distributeinfo=1';

    switch ($('show').value)
    {
      case 'coords':
        poststr += '&coords=1';
        break;
      case 'techs':
        poststr += '&techs=1';
        break;
      case 'all':
        poststr += '&coords=1&techs=1';
        break;
    }

    poststr += "&individual=" + ($('individual').checked ? "1" : "0") +
               "&template=" + style.GetStyle() +
               "&outputlang=" + $('outputlang').value +
               "&report=" + encodeURIComponent($('informe').value + "\n");

    return poststr;
  }

  function checkError(report)
  {
    errortag = report.getElementsByTagName('error');
    if (errortag.length > 0)
    {
      alert(errortag[0].firstChild.data);
      return true;
    }
    return false;
  }

  this.ReadyRequest = function(data)
  {
    style.Working(false);
    report = data[1].documentElement;

    if (checkError(report))
    {
      return false;
    }
    
    textNode = report.getElementsByTagName('report');
    if (textNode.length == 0)
    {
      alert('Internal error');
      return false;
    }

    text = textNode[0].firstChild.data;

    if (style.GetTemplate() != 'plain' && $('align').value == 'center')
    {
      text = '[center]' + text + '[/center]';
    }

    $('compactado').value = text;

    style.Update();

    return true;
  }


  /*
   * TODO: Esto tiene que cambiar por un método de comunicación XML cuanto antes.
   */
  this.Request = function()
  {
    poststr = MakePoststr();

    style.Working(true);
    ajax.Request(poststr);
  }
}

