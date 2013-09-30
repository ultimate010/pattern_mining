#!/usr/bin/perl
#文本替换成id并统计信息
#Zaks Wang
#2013-9-8
use Encode;
use utf8;
if(@ARGV != 1){
  print "Will produce word2Id.txt zhWord2Id.txt wordCount.txt\n";
  die "Usage: inputfile\n";
}
open(in,"$ARGV[0]");
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
  @arr = split(/[ \t]/,$line);
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
      $hashCount{$_}++;
    }else{
    }
  }
  if($flagZh == 1){
    ++$lineCount;
    $flagZh = 0;
  }else{
    $flagZh = 1;
  }
}
close(in);
print outFile encode("utf8","$lineCount\t$wordCount\t$wordTotal\n");
open(in,"$ARGV[0]");
while($line = <in>){
  chomp($line);
  $line = decode("utf8",$line);
  $line2 = <in>; chomp($line2);
  $line2 = decode("utf8",$line2);
  if(length($line) == 0 || length($line2) == 0){
    print "Wrong\n";
    next;
  }
  $line = $line."\t".$line2;
  @arr = split(/[ \t]/,$line);
  $first = 0;
  foreach(@arr){
    if(length($_) >= 1){
      if($first != 0){
        print outFile "\t";
      }else{
        $first = 1;
      }
      print outFile encode("utf8","$hashWordToId{$_}");
    }else{
    }
  }
  print outFile "\n";
}
close(in);
foreach(sort { $hashWordToId{$a} <=> $hashWordToId{$b} } keys %hashWordToId){
  print outDict encode("utf8","$_\t$hashWordToId{$_}\n");
}
foreach(sort { $hashZhToId{$a} <=> $hashZhToId{$b} } keys %hashZhToId){
  print outZhDict encode("utf8","$_\t$hashZhToId{$_}\n");
}
foreach(sort{ $hashCount{$a} <=> $hashCount{$b} } keys %hashCount){
  print outWordCount  encode("utf8","$_\t$hashCount{$_}\n");
}
close(outDict);
close(outZhDict);
close(outWordCount);
close(outFile);
