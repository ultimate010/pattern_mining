<?php
$item = $_REQUEST["item"]?$_REQUEST["item"]:"";
$found = "false";
if($item != ""){
  $str = '<table  border="0" cellspacing="0" cellpadding="0" class="table">
    <tr class=" label-primary">
    <th scope="col" width="50%" ><span style="color:white">模式</span></th>
    <th scope="col"><span style="color:white">频度(280万)</span></th>
    <th scope="col"><span style="color:white">关系强度</span></th>
    </tr>';
  $command = "grep '$item' ../classify_show/OUTDATA/* | head -n 15";
  unset($result);
  exec($command,$result);
  if(count($result) == 0){
  }else{
    $found = "true";
    for($i = 0;$i < count($result) && $i < 15;$i++){
      if($flag == 1){
	$str .= '<tr class="active" >';
	$flag = 0;
      }else{
	$str .= '<tr class="sucess">';
	$flag = 1;
      }
      $str .= '<td onclick="search(text(this))">计算机</td>';
      $str .='<td>1213</td><td>232.23423</td></tr>';
    }
    $str .= "</table>";
    echo $str;
  }
}
if($found == "false"){
  echo '<div class="alert alert-danger alert-dismissable">
    <button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
    <strong>注意!</strong> 没有该模式的结果，请修改后再次查询
    </div>';
}
?>

