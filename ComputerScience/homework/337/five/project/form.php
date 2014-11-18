<html>
<head>
<title>Form Results</title>
</head>
<body>
<ul>
    <li>Name: <?php echo $_POST["name"] ?> </li>
    <li>Email: <?php echo $_POST["email"] ?> </li>
    <li>Comment: <?php echo $_POST["comment"] ?> </li>
    <li>Newsletter: <?php echo $_POST["newsletter"] ?> </li>
</ul>

<?php
if( !empty($_POST) ) {
    $name = $_POST["name"];
    $email = $_POST["email"];
    $comment = $_POST["comment"];
    $newsletter = $_POST["newsletter"];
}
