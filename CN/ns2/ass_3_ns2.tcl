set ns [new Simulator]


set nf[ open out.nam w]
$ns namtrace-all $nf

set tf[open out.tr w]
$ns trace-all $tf

proc finish{} {
	global ns nf tf
	$ns flush-trace
	close $nf
	close $tf
	exec nam out.nam & exit 0

}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]


$ns duplex-link $n0 $n1 1Mb 10ms DropTail
$ns duplex-link $n0 $n2 1Mb 10ms DropTail
$ns duplex-link $n0 $n3 1Mb 10ms DropTail
$ns duplex-link $n0 $n4 1Mb 10ms DropTail
$ns duplex-link $n0 $n5 1Mb 10ms DropTail


set tcp [new Agent/TCP]
$tcp set class_ 1 
$ns attach-agent $n1 $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $n5 $sink

$ns connect $tcp $sink


set cbr [new Application/Traffic/CBR]
$cbr set packet_size_ 500
$cbr set interval_ 0.01
$cbr attach-agent $tcp


$ns at 1.0 "$cbr start"
$ns at 3.0 "$cbr stop"

$ns at 3.5 "finish"

$ns run



