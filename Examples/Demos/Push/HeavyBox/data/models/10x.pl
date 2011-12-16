#/usr/bin/perl

open(my $in, "<", "sphere.obj") or die "can't open input file!\n";
open(my $out, ">", "sphere10x.obj") or die "can't open output file!\n";

for my $line(<$in>) {
	if($line =~ /^v\s*(-?\.?\d+\.?\d*(?:e-?\d+)?)\s*(-?\.?\d+\.?\d*(?:e-?\d+)?)\s*(-?\.?\d+\.?\d*(?:e-?\d+)?)/) {
		my @first = split(/\./,$1);
		my @sec = split(/\./,$2);
		my @third = split(/\./,$3);
		my $firstStr = $first[0];
		my $secStr = $sec[0];
		my $thirdStr = $third[0];
		if(@first > 1) {
			$firstStr = $first[0].substr($first[1],0,1)."\.".substr($first[1],1).'0';
		}
		if(@sec > 1) {
			$secStr = $sec[0].substr($sec[1],0,1)."\.".substr($sec[1],1).'0';
		}
		if(@third > 1) {
			$thirdStr = $third[0].substr($third[1],0,1)."\.".substr($third[1],1).'0';
		}
		print $out "v $firstStr $secStr $thirdStr\n";
	}
	else {
		print $out $line;
	}
}
