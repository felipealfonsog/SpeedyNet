# SpeedyNet
SpeedyNet is a command-line utility for Linux and macOS that empowers users to accurately measure their Internet connection's download and upload speeds, providing real-time feedback on network performance.

SpeedyNet is a powerful and user-friendly command-line tool designed to assess the true capabilities of your Internet connection. Compatible with both Linux and macOS, SpeedyNet eliminates the need for web-based tests by directly measuring download and upload speeds in a terminal environment. Leveraging advanced network protocols and efficient data transmission techniques, SpeedyNet offers unparalleled accuracy and reliability.

With a comprehensive yet intuitive interface, SpeedyNet not only delivers real-time feedback on megabits per second (Mbps) download and upload speeds, but also provides insights into latency, packet loss, and other key technical metrics. Whether you're a home user curious about your network's performance or a professional seeking to optimize connectivity, SpeedyNet equips you with precise data to make informed decisions.

Experience the convenience of on-demand speed testing without relying on external servers or websites. SpeedyNet empowers you to gauge the health of your Internet link, troubleshoot issues, and verify service quality, all through a seamless command-line experience. Stay in control of your network performance with SpeedyNet.

## Features

### 1. Speed Test
- Measures download and upload speeds of the internet connection.
- Utilizes TCP sockets to simulate network traffic and calculate data transfer rates.
- Displays progress and completion percentage during the test.

### 2. Latency Test (Ping)
- Measures the round-trip time for data to travel between the user's device and a server.
- Calculates latency in milliseconds (ms) to indicate network responsiveness.

### 3. Packet Loss Test
- Estimates the percentage of data packets lost during transmission.
- Uses Internet Control Message Protocol (ICMP) Echo requests to simulate network traffic.
- Monitors the received ICMP Echo replies and calculates packet loss.

### 4. Server Selection
- Provides a list of predefined servers, including Google DNS, Cloudflare DNS, OpenDNS, Quad9 DNS, and CleanBrowsing DNS.
- Can fetch a list of servers from the Speedtest API to enable additional server options.

### 5. User-Friendly Interface
- Displays an ASCII art logo upon launching the utility.
- Shows progress indicators during tests to keep users informed about ongoing operations.
- Presents the results in a clear format with selected server, download/upload speeds, latency, and packet loss.

### 6. Error Handling
- Handles various network errors, such as socket creation, connection failures, and API requests.
- Provides descriptive error messages to guide users in understanding issues.

## Usage

1. Compile the program using a C compiler (e.g., GCC) and the provided source code.
2. Run the executable (`./speedynet`) to start the speed test utility.
3. The utility will perform the tests and display the results, including download/upload speeds, latency, and packet loss.

---

**Note:** SpeedyNet is a utility designed for testing purposes and may not provide results as accurate as dedicated speed test websites or applications.
#### NOTES

compiling:

```
gcc -o speedynet speedynet.c -lcurl
```


## Developer
- Developed by Felipe Alfonso González, a Computer Science Engineer from Chile.
- Contact: f.alfonso@res-ear.ch
- GitHub: [github.com/felipealfonsog](https://github.com/felipealfonsog)


### 7. MIT Licensed
- Distributed under the MIT License, allowing users to freely use, modify, and distribute the software.




