<?
  header("Content-type: text/html; charset=UTF-8");

  require_once('lang/main.php');
  $lang = getLang();
  require_once('lang/' . $lang);

  $p = strpos($lang, '.php');
  if ($p)
  {
    $langt = substr($lang, 0, $p);
  }
	  
  if (isset($_POST['email']) && isset($_POST['subject']) && isset($_POST['message']))
  {
    $email = $_POST['email'];
    $subject = $_POST['subject'];
    $message = $_POST['message'];

    if (!empty($email) && !empty($subject) && !empty($message))
    {
      mail('webmaster@ozipper.net', $subject, "From: {$email}\r\n\r\n{$message}",
           "From: \"Ozipper.net - Contact ({$email})\" <{$email}>\r\nReply-to: {$email}");
      header('Location: contact.php?sent=ok&lang=' . $langt);
      exit();
    }
  }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
  <head>
    <meta name="description" content="<?=T('meta_description')?>" />
    <meta name="keywords" content="<?=T('meta_keywords')?>" />

    <title>OZipper - <?=T('Contact')?></title>
    <link rel="stylesheet" type="text/css" href="style.css" />
    <script type="text/javascript"><!--
      function checkForm(form)
      {
        if (!form.email.value || !form.subject.value || !form.message.value)
	{
	  alert('<?=T('Please, fill all required data')?>');
	  return false;
	}

	return true;
      }
    // --></script>
  </head>
  <body>
    <?
      if (isset($_GET['sent']) && $_GET['sent'] == 'ok')
      {
    ?>
    <p align="center"><?=T('Mail sent')?></p>
    <?
      }
    ?>
    <p id="contact_advice">
      <img src="banner.png" alt="Banner" /><br />&nbsp;<br />
      <?=T('contact_advice')?>
    </p>

    <form action="contact.php?lang=<?=$langt;?>" method="post" onsubmit="return checkForm(this);">
    <table id="contact_form">
      <tr id="email">
        <td id="text"><?=T('Your email address:')?></td>
	<td id="content"><input type="text" name="email" /></td>
      </tr>
      <tr id="subject">
        <td id="text"><?=T('Subject:')?></td>
	<td id="content"><input type="text" name="subject" /></td>
      </tr>
      <tr id="message">
        <td id="text"><?=T('Message:')?></td>
	<td id="content"><textarea rows="15" cols="50" name="message"></textarea></td>
      </tr>
      <tr id="buttons">
        <td colspan="2"><input type="submit" value="<?=T('Submit')?>" />&nbsp;<input type="reset" value="<?=T('Reset')?>" /></td>
      </tr>
    </table>
    </form>

    <p>
      <a href="http://www.ozipper.net"><?=T('Return to OZipper.net')?></a>
    </p>
  </body>
</html>
