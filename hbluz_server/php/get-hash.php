<?php
header("Access-Control-Allow-Origin: *");
header("Content-Type: text/plain; charset=UTF-8");

$pwd = $_GET["pwd"];
echo MD5($pwd . "j%8[9}~MzE2XY4sq");

//j%8[9}~MzE2XY4sq
?>