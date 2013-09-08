#!/usr/bin/perl
#文本替换成id并统计信息
#Zaks Wang
#2013-9-8
use Encode;
use utf8;
open(outDict,">word2Id.txt");
open(outWordCount,">wordCount.txt");
open(outFile,">text.txt");

$wordTotal = 0;
$wordCount = 0;
$lineCount = 0;
while($line = <stdin>){
  #print $line;
  chomp($line);
  if(length($line) == 0){
    next;
  }
  ++$lineCount;
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
        print outFile "\t";
      }else{
        $first = 1;
      }
      print outFile encode("utf8","$hashWordToId{$_}");
      $hashCount{$_}++;
    }else{
    }
  }
  print outFile "\n";
}
print outFile encode("utf8","TotalLine:$lineCount\tDifWordCount:$wordCount\tTotalWords:$wordTotal\n");
foreach(sort { $hashWordToId{$a} <=> $hashWordToId{$b} } keys %hashWordToId){
  print outDict encode("utf8","Word2Id: key:$_\tvalue:$hashWordToId{$_}\n");
}

foreach(sort{ $hashCount{$a} <=> $hashCount{$b} } keys %hashCount){
  print outWordCount  encode("utf8","WordCount: key:$_\tvalue:$hashCount{$_}\n");
}
close(outDict);
close(outWordCount);
close(outFile);
