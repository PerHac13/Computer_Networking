set ns [new Simulator]

set nf [open out.nam w]
$ns namtrace-all $nf

set tf[open out.tr w]
$ns trace-all $tf

proc finish{}{

	global ns nf tf
	#ns flush-trace
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

$ns duplex-link $n0 $n1 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 2Mb 10ms DropTail
$ns duplex-link $n3 $n4 2Mb 10ms DropTail
$ns duplex-link $n4 $n5 2Mb 10ms DropTail
$ns duplex-link $n5 $n0 2Mb 10ms DropTail

set tcp [new Agent/TCP]
$tcp set class_ 1
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n3 $sink
$ns connect $tcp $sink

set cbr [new Application/Traffic/CBR]
$cbr set packet_size_ 500
$cbr set interval_ 0.01
$cbr attach-agent $tcp


$ns at 0.5 "$cbr start"
$ns at 4.5 "$cbr stop"
$ns at 5.0 "finish"


$ns run






