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
    .timeout(Duration::from_secs(5))
    .open();

match port {
    Ok(mut port) => {
        

        let mut serial_buf: Vec<u8> = vec![0; 1000];
        println!("Receiving data on {} at {} baud:", &port_name, &baud_rate);
        
        loop {
            
            match port.read(serial_buf.as_mut_slice()) {
                Ok(t) => 
                {
                    io::stdout().write_all(&serial_buf[..t]).unwrap();
                }
                ,
                Err(ref e) if e.kind() == io::ErrorKind::TimedOut => {println!("d");},
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