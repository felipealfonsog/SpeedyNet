### SpeedyNet 🌐🔗 [(Experimental Project)](#important-experimental-project)*

SpeedyNet is a command-line utility for Linux and macOS that empowers users to accurately measure their Internet connection's download and upload speeds, providing real-time feedback on network performance.

SpeedyNet is a powerful and user-friendly command-line tool designed to assess the true capabilities of your Internet connection. Compatible with both Linux and macOS, SpeedyNet eliminates the need for web-based tests by directly measuring download and upload speeds in a terminal environment. Leveraging advanced network protocols and efficient data transmission techniques, SpeedyNet offers unparalleled accuracy and reliability.

With a comprehensive yet intuitive interface, SpeedyNet not only delivers real-time feedback on megabits per second (Mbps) download and upload speeds, but also provides insights into latency, packet loss, and other key technical metrics. Whether you're a home user curious about your network's performance or a professional seeking to optimize connectivity, SpeedyNet equips you with precise data to make informed decisions.

Experience the convenience of on-demand speed testing without relying on external servers or websites. SpeedyNet empowers you to gauge the health of your Internet link, troubleshoot issues, and verify service quality, all through a seamless command-line experience. Stay in control of your network performance with SpeedyNet.

<sub>* This is currently an experimental phase where the primary focus is on making the system functional and establishing a practical and logical pathway that aligns with both my vision and the project's goals. It might contain errors, bugs, etc. Many other non-core elements of the project are considered secondary.</sub>

![Version](https://img.shields.io/github/release/felipealfonsog/SpeedyNet.svg?style=flat&color=blue)
![Main Language](https://img.shields.io/github/languages/top/felipealfonsog/SpeedyNet.svg?style=flat&color=blue)
[![Open Source? Yes!](https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/Naereen/badges/)

[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
<!--
[![GPL license](https://img.shields.io/badge/License-GPL-blue.svg)](http://perso.crans.org/besson/LICENSE.html)
-->

[![Vim](https://img.shields.io/badge/--019733?logo=vim)](https://www.vim.org/)
[![Visual Studio Code](https://img.shields.io/badge/--007ACC?logo=visual%20studio%20code&logoColor=ffffff)](https://code.visualstudio.com/)

#### Features

#### Speed Test
- Measures download and upload speeds of the internet connection.
- Utilizes TCP sockets to simulate network traffic and calculate data transfer rates.
- Displays progress and completion percentage during the test.

#### Latency Test (Ping)
- Measures the round-trip time for data to travel between the user's device and a server.
- Calculates latency in milliseconds (ms) to indicate network responsiveness.

#### Packet Loss Test
- Estimates the percentage of data packets lost during transmission.
- Uses Internet Control Message Protocol (ICMP) Echo requests to simulate network traffic.
- Monitors the received ICMP Echo replies and calculates packet loss.

#### Server Selection
- Provides a list of predefined servers, including Google DNS, Cloudflare DNS, OpenDNS, Quad9 DNS, and CleanBrowsing DNS.
- Can fetch a list of servers from the Speedtest API to enable additional server options.

#### User-Friendly Interface
- Displays an ASCII art logo upon launching the utility.
- Shows progress indicators during tests to keep users informed about ongoing operations.
- Presents the results in a clear format with selected server, download/upload speeds, latency, and packet loss.

#### Error Handling
- Handles various network errors, such as socket creation, connection failures, and API requests.
- Provides descriptive error messages to guide users in understanding issues.

#### Usage

1. Compile the program using a C compiler (e.g., GCC) and the provided source code.
2. Run the executable (`./speedynet`) to start the speed test utility.
3. The utility will perform the tests and display the results, including download/upload speeds, latency, and packet loss.

---

**Note:** SpeedyNet is a utility designed for testing purposes and may not provide results as accurate as dedicated speed test websites or applications.
#### NOTES

compiling in C:

get into the code source folder: 

```
cd src
```

```
gcc -o speedynet_c speedynet.c -lcurl && ./speedynet_c
```

compiling in C++:

```
clang++ -o speedynet_cpp speedynet.cpp -lcurl && ./speedynet_cpp
```

In macOS & Linux is important to install this library (Get Brew to get the package): 

```
brew install liboping
```

#### 📝Important (Experimental Project)

[![Experimental Project](https://img.shields.io/badge/Project-Type%3A%20Experimental-blueviolet)](#)

<p>This project is still in its experimental stage and may have limitations in terms of features and compatibility. Use at your own discretion.</p>

#### Developer
- Developed by Felipe Alfonso González, a Computer Science Engineer from Chile.
- Contact: f.alfonso@res-ear.ch
- GitHub: [github.com/felipealfonsog](https://github.com/felipealfonsog)

#### Support and Contributions

If you find this project helpful and would like to support its development, there are several ways you can contribute:

- **Code Contributions**: If you're a developer, you can contribute by submitting pull requests with bug fixes, new features, or improvements. Feel free to fork the project and create your own branch to work on.
- **Bug Reports and Feedback**: If you encounter any issues or have suggestions for improvement, please open an issue on the project's GitHub repository. Your feedback is valuable in making the project better.
- **Documentation**: Improving the documentation is always appreciated. If you find any gaps or have suggestions to enhance the project's documentation, please let me know.

[![Buy Me a Coffee](https://img.shields.io/badge/Buy%20Me%20a%20Coffee-%E2%98%95-FFDD00?style=flat-square&logo=buy-me-a-coffee&logoColor=black)](https://www.buymeacoffee.com/felipealfonsog)
[![PayPal](https://img.shields.io/badge/Donate%20with-PayPal-00457C?style=flat-square&logo=paypal&logoColor=white)](https://www.paypal.com/felipealfonsog)
[![GitHub Sponsors](https://img.shields.io/badge/Sponsor%20me%20on-GitHub-%23EA4AAA?style=flat-square&logo=github-sponsors&logoColor=white)](https://github.com/sponsors/felipealfonsog)

Your support and contributions are greatly appreciated! Thank you for your help in making this project better.


#### MIT Licensed
- Distributed under the MIT License, allowing users to freely use, modify, and distribute the software.




