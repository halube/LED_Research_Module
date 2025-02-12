<?php
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");

require("db.php");

if ($_SERVER['REQUEST_METHOD'] == 'POST' && empty($_POST))
    $_POST = json_decode(file_get_contents('php://input'), true);

$dsn = "mysql:host=$dbhost;dbname=$dbname;charset=$charset";
$opt = [
    PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES   => false,
];
$pdo = new PDO($dsn, $dbuser, $dbpass, $opt);
//SELECT * FROM programs WHERE user_id = :user_id
$stmt = $pdo->prepare('DELETE FROM `programs` WHERE id=:id'); //;
$id = $_POST["id"];
$stmt->execute(['id' => $id]);
?>