<?php
  if(isset($_POST['submit'])){
    print_r($_FILES);
    move_uploaded_file($_FILES['file']['tmp_name'],'C:\Users\Jason\OneDrive\AVLC\project\vlc\vlc-localization-master\vlc-localization-master\processing\jason.jpg');
    system('cd C:\Users\Jason\OneDrive\AVLC\project\vlc\vlc-localization-master\vlc-localization-master\processing\ && C:\Python27\python.exe run_jason.py');
  }
  echo "<hr/>";
  $fp = fopen('C:\Users\Jason\OneDrive\AVLC\project\vlc\vlc-localization-master\vlc-localization-master\processing\log.txt','r'); 
  if($fp){
    for($i=1;! feof($fp);$i++){
      echo fgets($fp). "<br />";
    } 
  }
  echo "<hr/>";
?>


<!DOCTYPE html>
<html>
<head>
</head>
<body>
  <form method="post" action="vlc.php" enctype="multipart/form-data">
    <input type="file" name="file" id="file" />
    <input type="submit" name="submit"/>
  </form>
  <img src="sample.jpg"/>
</body>
</html>