#!/usr/bin/perl
#删掉文本中的符号，将所有间隔符换为一个空格
#Zaks Wang
#2013-5-13
use Encode;
use utf8;
$DATAPATH ="/home/nlp/projects/SHAREDATA/merg/merge.txt";
$DATAOUTPAHT="/home/nlp/projects/SHAREDATA/merg/mergePurge.txt";
main();
sub main{
  @files = getFiles();
  chomp(@files);
  foreach (sort @files){
    print "Processing $_\n";
    open(in,"<:encoding(utf8)",$_) or die("Can't open file\n");
    $name=$_;
    $name=~s/$DATAPATH/$DATAOUTPAHT/gi;
    open(out,">$name");
    while($line=<in>){
      chomp($line);
#      print encode("utf8",$line."\n");
      $line=~s/[!@#\$\%\\\^&\*\(\)_\+\-=<>\?:"\{\}\|\~\,\.\/\;\'\[\]\`]//gi;
      $line=~s/[～［］／．–\！\@\#\￥\%……\&\*\（\）\——\+\{\}\|\：\“\”\’\《\》\？\~\【\】\、\；\‘\，\。\、\·]//gi;
      $line=~s/\s+/ /gi; #去掉空白
      print out encode("utf8",$line."\n");
    }
    close(in);
    close(out);
  }
}
#获取文件路径和名字
sub getFiles{
  $cmd='find '.$DATAPATH.' -name "*.txt"';
  print "$cmd\n";
  @files = readpipe($cmd);
  return @files;
}
