<?php

function user_session_prefix($name = null, $userid = null) {
	if(is_null($name)) {
		$name = $_SESSION['username'];
	}
	if(is_null($userid)) {
		$userid = $_SESSION['userid'];
	}
	return md5($name.$userid);
}

function user_is_logged_in() {	
	$sessid = session_id();
	if($sessid == "") {	
		return FALSE;
	}
	if(!isset($_SESSION) || !isset($_SESSION['username']) 
		|| !isset($_SESSION['userid']) /*|| !is_string($_SESSION['username'])*/ 
		|| /*!is_int($_SESSION['userid'])||*/ !$_SESSION['userid']) {
		//echo "problem here \n" . $_SESSION['userid'] ;		
		return FALSE;
	}
	$prefix = user_session_prefix($_SESSION['username'], $_SESSION['userid']);//user_session_prefix();
	if(strncmp($sessid, $prefix, strlen($prefix)) != 0) {
		//echo "problem here \n" . $prefix . "!=" . $sessid;		
		return FALSE;
	}
	return TRUE;
}

function user_is_admin() {
	return user_is_logged_in() && ($_SESSION["userid"] == 1);
}

function user_get_by_username($username) {
	global $project;
	$index = db_get_where($project['db'], 'user', array ('username' => $username));
	if($index <= 0)
		return null;
	$user = db_get($project['db'], 'user', $index);
	$user['index'] = $index;
	return $user;
}

function user_delete($user) {
	global $project;
	return db_delete($project['db'], 'user', $user['index']);
}

function user_add($user) {
	global $project;
	$user['created_on'] = time();
	return db_insert($project['db'], 'user', $user);
}

function user_save($user) {
	global $project;
	return db_update($project['db'], 'user', $user['index'], $user);
}

function user_visit($visitor, &$arg) {
	global $project;
	return db_visit($project['db'], 'user', $visitor, $arg);
}
