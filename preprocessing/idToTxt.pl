#!/usr/bin/perl
#还原id为文本
#Zaks Wang
#2013-9-122
use Encode;
use utf8;
if(@ARGV != 4){
  die "Usage: inputfile word2IdFile outfile skipWordCount\n";
}
open(txtFile,"$ARGV[0]");
open(in,"$ARGV[1]");
open(outFile,">$ARGV[2]");
$skip = $ARGV[3];

while($line = <in>){
  chomp($line);
  $line = decode("utf8",$line);
  if(length($line) == 0){
    next;
  }
  if($line=~/(.+)[ \t](.+)/){
    $hashIdtoWord{$2} = $1;
  }
}
while($line = <txtFile>){
  chomp($line);
  $line = decode("utf8",$line);
  $first = 1;
  $str = "";
  @arr = split(/[ \t]/,$line);
  $count = 0;
  for($i = 0;$i < @arr;$i++){
    if(++$count <= $skip){
      next;
    }else{
      $count = 0;
    }
    if($first == 1){
      $first = 0;
    }else{
      $str.="\t";
    }
    $str = $str.$hashIdtoWord{$arr[$i]};
  }
  $str.="\n";
  print outFile encode("utf8",$str);
}
close(in);
close(txtFile);
close(outFile);
