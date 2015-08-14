<?php

function stringCat($s1, $s2) {
  return $s1 . $s2;
}

/* setErrorLevel */
function setErrorLevel() {
	error_reporting( E_ERROR | E_WARNING | E_NOTICE | E_PARSE );
}

/* myStringCleanup */
function myStringCleanup($s1) {
	return strtolower( preg_replace( '/\s+/', '', $s1 ) );
}

/* oddEven */
function oddEven( $number ) {
	return (($number % 2) == 0) ? "even" : "odd";
}

/* buildArray */
function buildArray( $first, $second, $third, $fourth ) {
	return array( $first, $second, $third, $fourth );
}

/* readLocalFile */
function readLocalFile() {
	$file = fopen( "external_file.txt", "r" );
	$contents = fread( $file, filesize("external_file.txt") );
	fclose( $file );
	return preg_replace( '/\n/', '<br>', $contents );
}

/* serverSoftware */
function serverSoftware() {
	return $_SERVER['SERVER_SOFTWARE'];
	//return phpinfo( INFO_GENERAL );
}

/* makeObject */
function makeObject( $secret ) {
	return new mySimpleClass( $secret );
}

class mySimpleClass {
  /* $secret property */
  private $secret;

  public function __construct($s) {
    /* Store a copy of the incoming secret */
    $this->secret = $s;
  }
  
  public function getSecret() {
    /* Return a base 64 encoded version of the stored secret */
    return base64_encode( $this->secret );
  }
}

/* getDatatype */
function getDatatype( $variable ) {
	return gettype( $variable );
}
