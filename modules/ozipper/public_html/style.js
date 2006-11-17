Style = function(template_, background_)
{
  var template = template_;
  var background = background_;

  function $(id)
  {
    return document.getElementById(id);
  }

  this.GetTemplate = function()
  {
    return template;
  }

  this.GetBackground = function()
  {
    return background;
  }

  this.GetStyle = function()
  {
    if (template == 'plain')
    {
      return 'plain';
    }
    else
    {
      return template + '_' + background;
    }
  }

  this.SetTemplate = function(template_)
  {
    template = template_;
  }

  this.SetBackground = function(background_)
  {
    background = background_;
  }

  this.Update = function()
  {
    if (background == 'dark')
    {
      $('wysiwyg').className = 'wysiwyg_dark';
    }
    else if (background == 'light' || template == 'plain')
    {
      $('wysiwyg').className = 'wysiwyg_light';
    }

    $('wysiwyg').innerHTML = parseCode($('compactado').value, template);
  }

  this.Working = function(state)
  {
    $('result').style.display = (state ? 'none' : '');
    $('busy').style.display   = (state ? '' : 'none');
    $('submit').disabled      = (state ? true : false);
  }
}

