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
$stmt = $pdo->prepare('INSERT INTO `programs` (`id`, `user_id`, `program_name`, `date_posted`, `is_autosave`, `raw_json`) VALUES (NULL, :user_id, :program_name, FROM_UNIXTIME(:date_posted), :is_autosave, :raw_json)'); //;
$user_id = $_POST["user_id"];
$program_name = $_POST["program_name"];
$date_posted = $_POST["date_posted"];
$is_autosave = $_POST["is_autosave"];
$raw_json = json_encode($_POST["raw_json"]);
$stmt->execute(['user_id' => $user_id, 'program_name' => $program_name, 'date_posted' => time(), 'is_autosave' => $is_autosave, 'raw_json' => $raw_json]);
?>