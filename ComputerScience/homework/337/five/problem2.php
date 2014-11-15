<!doctype html>
<html>
<head>
  <title>Assignment #5 - Problem 2</title>
  <style>
    body {
      font-family: sans-serif;
      width: 500px;
      margin: 0 auto;
      padding: 0;
      background-color: #EEE;
    }
    header ul {
      margin: 0;
      padding: 0;
    }
    header li {
      display: block;
      box-sizing: border-box;
      width: 100px;
      padding: 15px;
      text-align: center;
      margin: 10px;
      float: left;
      background-color: #AAA;
    }
    .clearfix {
      clear: both;
    }
  </style>
</head>

<body>
  <!-- Include the header.hmtl file here -->
  <?php include 'header.html' ?>
  
  <section id="main">
    <h2>Today is <?php echo date("l") . ', ' . date('F') . ' ' . date ('d') . ' ' . date('Y') ?></h2>
  </section>
</body>
</html>
