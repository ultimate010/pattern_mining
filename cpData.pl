#!/usr/bin/perl
#Zaks Wang
#2013-5-12
#��log�ļ���Ȼ�󿽱���ȷ���ļ�
$LOGFILE="/home/nlp/git_projs/pattern_mining/log.txt";
$OUTPATH="/home/nlp/data_out/";
@files=();
main();


sub main{
  open(IN,$LOGFILE);
  while(<IN>){
    chomp;
    if($_=~/^(\S+)\s+(\S+)$/){
      print "$1\t$2\n";
      push(@files,$2);
    }
  }
  close(IN);
  foreach (sort @files){
    $cmd ="cp $_ $OUTPATH";
    system($cmd);
    print "$_\n";
  }
}

