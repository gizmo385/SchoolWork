<!doctype html>
<html>
<head>
  <title>Assignment #5 - Problem 1</title>
  <?php include 'problem1_library.php'; ?>
  <style>
    table {
      width: 40em;
      border-collapse: collapse;
      margin: 0 auto;
    }
    
    th, td {
      padding: 0.5em;
    }
    
    td {
      border: 1px solid gray;
    }
  </style>
</head>

<body>

  <table>
    <tr>
      <th>Step</th>
      <th>Result</th>
    </tr>
    
    <tr>
      <td>String Concat</td>
      <td><?php echo stringCat('Hello', 'World'); ?></td>
    </tr>

    <tr>
      <td>Set Error Reporting</td>
      <td><?php 
          setErrorLevel();
          echo error_reporting(); 
          ?>
      </td>
    </tr>

    <tr>
      <td>String Cleanup</td>
      <td>[<?php 
          echo myStringCleanup("   Correct Horse Battery Staple    ");
          ?>]
      </td>
    </tr>

    <tr>
      <td>Odd or Even</td>
      <td>
          <?php
            for ($i = 0; $i < 5; $i++) {
              echo "$i " . oddEven($i) . "<br>";
            }
          ?>
      </td>
    </tr>
 
     <tr>
      <td>Build Array</td>
      <td>
          <pre>
          <?php print_r(buildArray("One", "Two", "Three", "Four")); ?>
          </pre>
      </td>
    </tr>
 
    <tr>
      <td>Read Local File</td>
      <td><?php echo readLocalFile(); ?></td>
    </tr>

    <tr>
      <td>Server Software</td>
      <td><?php echo serverSoftware(); ?></td>
    </tr>

    <tr>
      <td>Make Object</td>
      <td>
          <pre>
          <?php 
            $obj = makeObject("Honey Pot");
            print_r($obj); 
            echo $obj->getSecret();
          ?>
          </pre>
      </td>
    </tr>

    <tr>
      <td>Data Types</td>
      <td>
          <?php 
            echo getDatatype(1) . "<br>";
            echo getDatatype(3.1415) . "<br>";
            echo getDatatype("What") . "<br>";
            echo getDatatype(new StdClass()) . "<br>";
            echo getDatatype(true) . "<br>";
          ?>
      </td>
    </tr>

  </table>

</body>
</html>
