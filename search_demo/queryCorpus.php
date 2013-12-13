<?php
$item = $_REQUEST["item"]?$_REQUEST["item"]:"";
$url = curPageURL();
$found = "false";
if($item != ""){
	$str = '
		<table  border="0" cellspacing="0" cellpadding="0" class="table">
		<tr class=" label-primary">
		<th scope="col" width="100%" ><span style="color:white">检索结果</span></th>
		</tr>
		';
	$command = "grep '$item' ../classify_show/OUTDATA/* | head -n 40";
	unset($result);
	exec($command,$result);
	if(count($result) == 0){
	}else{
		$found = "true";
		for($i = 0;$i < count($result) && $i < 40;$i++){
			if($flag == 1){

				$str .= '
					<tr class="active">
					<td>';
				$flag = 0;
			}else{
				$str .= '
					<tr class="sucess">
					<td>';
				$flag = 1;
			}
			$str .= $result[$i];
			$str .='</td>
				</tr>';
		}
		$str .= "
			</table>
			";
		echo $str;
	}
}
if($found == "false"){
	echo '<div class="alert alert-danger alert-dismissable">
		<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
		<strong>注意!</strong> 没有该查询字串的结果，请修改后再次查询
		</div>';
}
?>

<?php

function curPageURL()
{
	$pageURL = 'http';

	if ($_SERVER["HTTPS"] == "on")
	{
		$pageURL .= "s";
	}
	$pageURL .= "://";

	$this_page = $_SERVER["REQUEST_URI"];

	if (strpos($this_page, "?") !== false)
		$this_page = reset(explode("?", $this_page));

	if ($_SERVER["SERVER_PORT"] != "80")
	{
		$pageURL .= $_SERVER["SERVER_NAME"] . ":" . $_SERVER["SERVER_PORT"] . $this_page;
	}
	else
	{
		$pageURL .= $_SERVER["SERVER_NAME"] . $this_page;
	}
	return $pageURL;
}
?>
