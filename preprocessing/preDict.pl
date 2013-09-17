#!/usr/bin/perl
#文本替换成id并统计信息
#Zaks Wang
#2013-9-8
use Encode;
use utf8;
open(in,"test.file");
open(outDict,">word2Id.txt");
open(outZhDict,">zhWord2Id.txt");
open(outWordCount,">wordCount.txt");
open(outFile,">text.txt");

$wordTotal = 0;
$wordCount = 0;
$lineCount = 0;
$flagZh = 1;
while($line = <in>){
  #print $line;
  chomp($line);
  $line = decode("utf8",$line);
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
        if($flagZh == 1){
          $hashZhToId{$_} = $wordCount;
        }
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
  if($flagZh == 1){
    $flagZh = 0;
  }else{
    $flagZh = 1;
  }
  print outFile "\n";
}
print outFile encode("utf8","TotalLine:$lineCount\tDifWordCount:$wordCount\tTotalWords:$wordTotal\n");
foreach(sort { $hashWordToId{$a} <=> $hashWordToId{$b} } keys %hashWordToId){
  print outDict encode("utf8","$_\t$hashWordToId{$_}\n");
}
foreach(sort { $hashZhToId{$a} <=> $hashZhToId{$b} } keys %hashZhToId){
  print outZhDict encode("utf8","$_\t$hashZhToId{$_}\n");
}
foreach(sort{ $hashCount{$a} <=> $hashCount{$b} } keys %hashCount){
  print outWordCount  encode("utf8","$_\t$hashCount{$_}\n");
}
close(in);
close(outDict);
close(outZhDict);
close(outWordCount);
close(outFile);
