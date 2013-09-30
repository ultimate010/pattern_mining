#!/usr/bin/perl
#处理mahout seq序列为可读序列
use Encode;
use utf8;
open(in,"$ARGV[0]");
open(out,">$ARGV[1]");
$line = <in>;
print $line;
$line = <in>;
print $line;
while($line = <in>){
  chomp $line;
  if($line =~/^Key:.+Value: (.+)/){
    $line = $1;
    @arr = $line =~/(\([^\)]+\))/g;
    for($i = 0;$i < @arr;$i++){
      $line = $arr[$i];
      if($line =~/\(\[([^\]]+)\]\,([0-9]+)\)/){
        $line = $1; $num = $2;
        $line =~s/,//g;
        #print out "$line --  $num\n";
        if(defined $hash{$line}){
          if($num > $hash{$line}){
            $hash{$line} = $num;
          }
        }else{
          $hash{$line} = $num;
        }
      }
    }
  }
}
foreach (sort{$hash{$b} <=> $hash{$a}} keys %hash){
  $key = $_;
  @arr = $key =~/ /g;
  if(@arr < 1){
  }else{
    #print out "$_ :$hash{$_}\n";
    print out "$_\n";
  }
}
close(out);
close(in);
