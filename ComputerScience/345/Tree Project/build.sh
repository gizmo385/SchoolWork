#!/usr/bin/env bash
javac -cp . Tree.java MasterTest.java
java -cp ./junit.jar org.junit.runner.JUnitCore ./MasterTest
