<?
  $langs = array(
                 'en'		=> 'en.php',
		 'en-us'	=> 'en.php',
		 'en-gb'	=> 'en.php',
		 'es'		=> 'es.php',
		 'es-es'	=> 'es.php',
		 'de'	        => 'de.php',
		 'de-de'	=> 'de.php'
                );

  function T($str)
  {
    global $langarr;

    if ($langarr[$str])
    {
      return $langarr[$str];
    }
    else
    {
      return $str;
    }
  }

  function availLang($lang)
  {
    global $langs;

    if (!$langs[$lang])
    {
      return false;
    }
   
    return true;
  }

  function getLang()
  {
    global $_COOKIE;
    global $_SERVER;
    global $langs;

    if (isset($_GET['lang']))
    {
      if (availLang($_GET['lang']))
      {
        return $langs[$_GET['lang']];
      }
    }
    else if (isset($_COOKIE['lang']))
    {
      if (availLang($_COOKIE['lang']))
      {
        return $langs[$_COOKIE['lang']];
      }
    }

    $l = split(',', $_SERVER['HTTP_ACCEPT_LANGUAGE']);
    foreach ($l as $current)
    {
      $cur = split(';', $current);
      if (availLang($cur[0]))
      {
        return $langs[$cur[0]];
      }
    }

    reset($langs);
    $default = each($langs);
    return $default['value'];
  }
?>
