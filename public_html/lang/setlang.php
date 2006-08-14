<?
  if (isset($_GET['lang']))
  {
    if (!setcookie('lang', $_GET['lang'], time() + (86400 * 7), '/', 'ozipper.net'))
    {
      echo "Error seteando cookie";
    }
  }
  header("Vary: cookie");
  header("Connection: close");

  header("Location: ../");
?>
