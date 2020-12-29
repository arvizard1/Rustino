use std::io::{self, Write};
use std::time::Duration;

fn main() {
    let ports = serialport::available_ports().expect("No ports found");
    for p in ports
    {
        println!("{}", p.port_name);
    }
    let port_name = "/dev/tty.usbmodem14701";
    let baud_rate = 9600;
    let port = serialport::new(port_name, baud_rate)
    .timeout(Duration::from_millis(500))
    .open();

match port {
    Ok(mut port) => {
        let mut serial_buf = [0; 1];
        const END: u8 = 90;
        println!("Receiving data on {} at {} baud:", &port_name, &baud_rate);
        loop {
            match port.read(&mut serial_buf) {
                Ok(t) => 
                {
                    if &serial_buf[0] == &END {break};
                    
                    io::stdout().write_all(&serial_buf[..t]).unwrap();
                }
                ,
                Err(ref e) if e.kind() == io::ErrorKind::TimedOut => {
                    port.write("g".as_bytes()).expect("Send failed");
                },
                Err(e) => eprintln!("{:?}", e),
            }
        }
    }
    Err(e) => {
        eprintln!("Failed to open Error: {}", e);
        ::std::process::exit(1);
    }
}
    
}