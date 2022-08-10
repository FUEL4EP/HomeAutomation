The set-up of an automatic ProwlApp radiation alarm requires

1. a Homematic IP alarm siren [HmIP-ASIR](https://de.elv.com/elv-homematic-ip-arr-bausatz-alarmsirene-hmip-asir-fuer-smart-home-hausautomation-151232)
2. a howto use the alarm siren can be found [here](https://technikkram.net/blog/2017/08/22/problemloesung-homematic-ip-alarmsirene-an-einer-ccu2-oder-raspberrymatic-betreiben/)
3. an Apple iPhone or/and iPAd
4. installation of the [ProwlAPP](https://apps.apple.com/de/app/prowl-easy-push-notifications/id320876271) from Apple's AppStore
5. set-up a [homematic program](./Homematic_proogram_definition.png) with [embedded homematic script](./Prowl_radioactivity_alarm.hmscr)
6. please change in the embedded homematic scripts the settings to your personal ones:

> string NotifyMyProwlApiKey='xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx';                          ! please insert your personal Prowl API key here
> 
> integer NotifyMyProwlMsgPriority=2;                                                             ! highest priority = 2 lowest priority = -2 in ProwlAPP
> 
> string NotifyMyProwlMsgApplication="Raspberrymatic%20Your_City%20Your_radiation_sensor_name";   ! insert your ProwlApp alarm headline here 
> string NotifyMyProwlMsgEvent="Alarm%20:%20";
> 
> 
> string Channel_Selector = 'Radioaktivitätssensor 1';                                            ! insert your Homematic device name here, space is not '%20' !

 
6. don't forget to test the correct function, turn on the debug mode of the embedded homematic script for that purpose
7. use is on your own risk