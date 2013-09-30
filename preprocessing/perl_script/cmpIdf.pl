#!/usr/bin/perl
#comput word idf
#Zaks Wang
#2013-9-28
use Encode;

if(@ARGV != 3){
  die "Usage: inputfile outfile encode\n";
}
open(input,"$ARGV[0]");
open(output,">$ARGV[1]");
$encode = $ARGV[2];
$lineCount = 0;
while($line = <input>){
  chomp $line;
  ++$lineCount;
  $line = decode("$encode",$line);
  @arr = split("[ \t]",$line);
  %hashSent = ();
  foreach (@arr){
    $hashSent{$_} = 1;
  }
  foreach (sort keys %hashSent){
    $hash{$_}++;
  }
}
foreach $key (sort{$hash{$b} <=> $hash{$a}} keys %hash){
  $pro = log($lineCount/$hash{$key});
  print output encode("$encode","$key\t$pro\n");
}
close(input);
close(output);
