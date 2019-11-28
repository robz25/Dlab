R"rawText(

<!DOCTYPE html>
<!-- Local site to change lamp operation mode -->
<html>
<head>
  <title>Tech Jam 2019 Trophy</title>
  <!--<link rel="stylesheet" type="text/css" href="style.css">-->
  <style type="text/css">
    @font-face {
    font-family: 'eurostyle';
    src: url('Eurostile-LT-Std-Bold-Oblique.h');
}
button:hover{
  color:green;
  background-color: #aaa
}
h1{
  font-family: eurostyle, sans-serif;

}
.button{
  width: 125px;
  padding: 10px 10px;
  background-color: rgb(117, 108, 122);
  color:rgb(255, 255, 255);
  border-radius: 30px;
}
a{
  width: 125px;
  padding: 10px 10px;
  background-color: #fff;
  color:rgb(255, 0, 0);
  border-radius: 30px; 
}
.container{
  width: 80%;
  margin: auto;
}
.box{
  text-align: center;
  background-color: #25BFFF;
  border: 7px orange solid;
  padding-bottom: 35px;
}
  </style>
</head>
<body>
  <div class="container">
    <div class="box">
    <h1>Trophy configuration</h1>
    <!--<br>
   <form  action="maco">
        <input class="button" type="submit" value="Manual colour">
    </form>-->
    <br>
    <form  action="akco">
        <input class="button" type="submit" value="Akamai colors">
    </form>
    <br>
    <form  action="rgb">
        <input class="button" type="submit" value="RGB">
    </form>
    <br>
    <form  action="sf">
        <input class="button" type="submit" value="Slow fade">
    </form>
    <br>
    <form  action="ff">
        <input class="button" type="submit" value="Fast fade">
    </form>
    </div>
  </div>
</body>
</html>
)rawText"
