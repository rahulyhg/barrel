<?php

// chown www-data:www-data -R .data
define('FILEDB_HOME', '.data');

function filedb_writefile($file, $x) {
	if(!($fd = @fopen($file, 'w'))) {
		return FALSE;
	}
	foreach($x as $key=>$val) {
		@fputs($fd, $key.' = '.$val."\r\n");
	}
	@fclose($fd);
	return TRUE;
}

function filedb_readfile($file) {
	if(($fd = @fopen($file, 'r')) == FALSE) {
		return FALSE;
	}
	$x = array();
	while(($str = @fgets($fd))) {
		$i = strpos($str, '=');
		if($i == 0 || $i == FALSE) {
			continue;
		}
		$key = substr($str, 0, $i);
		$key = trim($key);
		$val = substr($str, $i+1);
		$x[$key] = $val;
	}
	@fclose($fd);
	return $x;
}

function filedb_create_or_prepare_table($db, $tbl) {
	if(!@file_exists(FILEDB_HOME) && !@mkdir('.data', 0777)) {
		return;
	}
	$dbdir = FILEDB_HOME.'/'.make_it_txt($db);
	if(!@file_exists($dbdir) && !@mkdir($dbdir, 0777)) {
		return;
	}
	$tbldir = $dbdir.'/'.make_it_txt($tbl);
	if(!@file_exists($tbldir) && !@mkdir($tbldir, 0777)) {
		return;
	}
	return $tbldir;
}

function filedb_database_insert($db, $tbl, $more) {
	// mkdir .data
	// mkdir .data $db
	// mkdir .data $db/$tbl
	// echo $content > .data $db/$tbl/$index.data
	
	$tbldir = filedb_create_or_prepare_table($db, $tbl);
	if(!$tbldir) {
		error_report("Could not create table dir, please check file permission\n");
		return FALSE;
	}
	$index = 1;
	$infofile = $tbldir."/.info";
	$info = filedb_readfile($infofile);
	if(!isset($info) || !isset($info['index'])) {
		// new table
		$info = array('index' => $index);
	} else {
		$info['index'] = intval($info['index']);
		$info['index']++;
		$index = $info['index'];
	}
	// TODO write a function that locks and updates index file ..
	if(!filedb_writefile($infofile, $info) || !filedb_writefile($tbldir."/".$index, $more)) {
		return FALSE;
	}
	return $index;
}

function filedb_database_get($db, $tbl, $index) {
	$dbdir = FILEDB_HOME.'/'.make_it_txt($db);
	$tbldir = $dbdir.'/'.make_it_txt($tbl);
	$output = filedb_readfile($tbldir."/".$index);
	return $output;
}

function filedb_database_delete($db, $tbl, $index) {
}

function filedb_database_register() {
	return array(
		'insert' => 'filedb_database_insert',
		'delete' => 'filedb_database_delete',
		'get' => 'filedb_database_get',
	);
}

