#!/usr/bin/perl
#2013-9-29
use Encode;
use utf8;
if(@ARGV != 3){
  die "Usage: lemfile inputfile outfile\n";
}
open(lem,"$ARGV[0]");
open(in,"$ARGV[1]");
open(out,">$ARGV[2]");
while($line = <lem>){
  chomp $line;
  $line = decode("utf8",$line);
  @arr = $line=~/\S+/g;
  for($i = 1;$i < @arr;$i++){
    $hash{$arr[$i]} = $arr[0];
    #print $arr[$i];
  }
}
while($line = <in>){
  chomp $line;
  $line = decode("utf8",$line);
  $line2 = <in>;
  chomp $line2;
  $line2 = decode("utf8",$line2);
  @arr = $line2=~/\S+/g;
  for($i = 0;$i < @arr;$i++){
    if(defined $hash{$arr[$i]}){
      $arr[$i] = $hash{$arr[$i]};
    }
  }
  $line2 = $arr[0];
  for($i = 1;$i < @arr;$i++){
    $line2 = $line2." ".$arr[$i];
  }
  print out encode("utf8","$line\n");
  print out encode("utf8","$line2\n");
}
close(in);
close(out);
