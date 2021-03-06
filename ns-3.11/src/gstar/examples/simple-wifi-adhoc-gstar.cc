#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/gstar-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SimpleWifiAdhocExample");

int main (int argc, char *argv[])
{
	LogComponentEnable("SimpleWifiAdhocExample",LOG_LEVEL_ALL);
	 
	CommandLine cmd;
	cmd.Parse (argc, argv);

	NS_LOG_INFO("Create nodes");
	NodeContainer c;
  	c.Create (2);	  

	MobilityHelper mobility;
  	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  	positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  	positionAlloc->Add (Vector (100.0, 0.0, 0.0));
	//positionAlloc->Add (Vector (100.0, 0.0, 0.0));
 	mobility.SetPositionAllocator (positionAlloc);
  	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  	mobility.Install (c);


	//MobilityHelper mobility;
  	//.SetPositionAllocator ("ns3::GridPositionAllocator",
        //	                       "MinX", DoubleValue (0.0),
          //                             "MinY", DoubleValue (0.0),
            //                           "DeltaX", DoubleValue (100),
              //                         "DeltaY", DoubleValue (0),
                //                       "GridWidth", UintegerValue (2),
                  ///                     "LayoutType", StringValue ("RowFirst"));

	//Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  	//positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  	//positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  	//mobility.SetPositionAllocator (positionAlloc);
  	//mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  	//mobility.Install (c);

	NetDeviceContainer devices;
	NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  	wifiMac.SetType ("ns3::AdhocWifiMac");
  	YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  	YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  	wifiPhy.SetChannel (wifiChannel.Create ());
  	WifiHelper wifi = WifiHelper::Default ();
  	wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"),"RtsCtsThreshold", UintegerValue (0));
  	devices = wifi.Install (wifiPhy, wifiMac, c); 

	 // Enable GSTAR
    	NS_LOG_INFO ("Enabling GSTAR Routing.");
  	GSTARHelper gstar;

  	Ipv4StaticRoutingHelper staticRouting;

  	Ipv4ListRoutingHelper list;

  	list.Add (staticRouting, 0);
  	list.Add (gstar, 0);

  	InternetStackHelper internet;
  	internet.SetRoutingHelper (list); // has effect on the next Install ()
  	internet.Install (c);

	// Later, add IP addresses.
	NS_LOG_INFO ("Assign IP Addresses.");
  	Ipv4AddressHelper ipv4;
  	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  	Ipv4InterfaceContainer i01 = ipv4.Assign (devices);

	Simulator::Stop (Seconds (24));
        NS_LOG_INFO ("Run Simulation.");

  	Simulator::Run ();
  	Simulator::Destroy ();
        NS_LOG_INFO ("Done");  	
	return 0;
}

	



