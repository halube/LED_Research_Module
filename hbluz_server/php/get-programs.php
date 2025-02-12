<?php
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");

require("db.php");

$dsn = "mysql:host=$dbhost;dbname=$dbname;charset=$charset";
$opt = [
    PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES   => false,
];
$pdo = new PDO($dsn, $dbuser, $dbpass, $opt);

$stmt = $pdo->prepare('SELECT * FROM programs WHERE user_id = :user_id ORDER BY date_posted DESC');
$user_id = $_GET["user_id"];
$stmt->execute(['user_id' => $user_id]);
$result=$stmt->fetchAll(PDO::FETCH_ASSOC);
print json_encode($result);
?>