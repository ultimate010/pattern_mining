#!/usr/bin/perl
#删掉文本中的符号，将所有间隔符换为一个空格
#Zaks Wang
#2013-5-13
use Encode;
use utf8;
$wordTotal = 0;
$wordCount = 0;
while($line = <stdin>){
  #print $line;
  chomp($line);
  if(length($line) == 0){
    next;
  }
  @arr = split(/[ \t]/,$line);
  $first = 0;
  foreach(@arr){
    if(length($_) >= 1){
      $wordTotal++;
      if(defined $hashCount{$_}){
      }else{
        $hashWordToId{$_} = $wordCount;
        $wordCount++;
      }
      if($first != 0){
        print "\t";
      }else{
        $first = 1;
      }
      print "$hashWordToId{$_}";
      $hashCount{$_}++;
    }else{
    }
  }
  print "\n";
}
foreach(sort keys %hashWordToId){
  print "Word2Id: key:$_ value:$hashWordToId{$_}\n";
}

foreach(sort keys %hashCount){
  print "WordCount: key:$_ value:$hashCount{$_}\n";
}

