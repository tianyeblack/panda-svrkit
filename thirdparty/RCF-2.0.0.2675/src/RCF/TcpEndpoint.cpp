
//******************************************************************************
// RCF - Remote Call Framework
//
// Copyright (c) 2005 - 2012, Delta V Software. All rights reserved.
// http://www.deltavsoft.com
//
// RCF is distributed under dual licenses - closed source or GPL.
// Consult your particular license for conditions of use.
//
// If you have not purchased a commercial license, you are using RCF 
// under GPL terms.
//
// Version: 2.0
// Contact: support <at> deltavsoft.com 
//
//******************************************************************************

#include <RCF/TcpEndpoint.hpp>

#include <boost/config.hpp>

#include <RCF/InitDeinit.hpp>
#include <RCF/SerializationProtocol.hpp>

#include <RCF/TcpAsioServerTransport.hpp>
#include <RCF/TcpClientTransport.hpp>

#ifdef RCF_USE_SF_SERIALIZATION
#include <SF/Registry.hpp>
#include <SF/SerializeParent.hpp>
#endif

namespace RCF {

    TcpEndpoint::TcpEndpoint()
    {}

    TcpEndpoint::TcpEndpoint(int port) :
        mIpAddress("127.0.0.1", port)
    {}

    TcpEndpoint::TcpEndpoint(const std::string &ip, int port) :
        mIpAddress(ip, port)
    {}

    TcpEndpoint::TcpEndpoint(const IpAddress & ipAddress) :
        mIpAddress(ipAddress)
    {}

    TcpEndpoint::TcpEndpoint(const TcpEndpoint &rhs) :
        mIpAddress(rhs.mIpAddress)
    {}

    EndpointPtr TcpEndpoint::clone() const
    {
        return EndpointPtr(new TcpEndpoint(*this));
    }

    std::string TcpEndpoint::getIp() const
    {
        return mIpAddress.getIp();
    }

    int TcpEndpoint::getPort() const
    {
        return mIpAddress.getPort();
    }

    std::string TcpEndpoint::asString() const
    {
        std::ostringstream os;
        std::string ip = getIp();
        if (ip.empty())
        {
            ip = "127.0.0.1";
        }
        os << "TCP endpoint " << ip << ":" << getPort();
        return os.str();
    }

    IpAddress TcpEndpoint::getIpAddress() const
    {
        return mIpAddress;
    }

#ifdef RCF_USE_SF_SERIALIZATION

    void TcpEndpoint::serialize(SF::Archive &ar)
    {
        // TODO: versioning.
        // ...

        serializeParent( (I_Endpoint*) 0, ar, *this);
        ar & mIpAddress;
    }

#endif

    std::auto_ptr<I_ServerTransport> TcpEndpoint::createServerTransport() const
    {
        return std::auto_ptr<I_ServerTransport>(
            new RCF::TcpAsioServerTransport(mIpAddress));
    }

    std::auto_ptr<I_ClientTransport> TcpEndpoint::createClientTransport() const
    {
        return std::auto_ptr<I_ClientTransport>(
            new RCF::TcpClientTransport(mIpAddress));
    }

#ifdef RCF_USE_SF_SERIALIZATION

    void initTcpEndpointSerialization()
    {

        SF::registerType( (TcpEndpoint *) 0, "RCF::TcpEndpoint");
        SF::registerBaseAndDerived( (I_Endpoint *) 0, (TcpEndpoint *) 0);
    }

#endif

} // namespace RCF
