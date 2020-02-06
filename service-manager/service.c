[Unit]
Description=start service manager on boot

[Service]
Type=simple
ExecStart=/whateverThePathIs/service.c

[Install]
WantedBy=multi-user.target