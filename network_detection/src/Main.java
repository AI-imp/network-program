import org.snmp4j.CommunityTarget;
import org.snmp4j.PDU;
import org.snmp4j.Snmp;
import org.snmp4j.TransportMapping;
import org.snmp4j.event.ResponseEvent;
import org.snmp4j.mp.SnmpConstants;
import org.snmp4j.smi.Address;
import org.snmp4j.smi.GenericAddress;
import org.snmp4j.smi.OID;
import org.snmp4j.smi.OctetString;
import org.snmp4j.smi.*;
import org.snmp4j.smi.VariableBinding;
import org.snmp4j.transport.DefaultUdpTransportMapping;

public class Main {

    public static void main(String[] args) {
        try {
            TransportMapping transport = new DefaultUdpTransportMapping();
            Snmp snmp = new Snmp(transport);
            transport.listen();
            Address targetAddress = GenericAddress.parse("udp:192.168.0.1/161"); // 替换为目标设备的IP地址
            CommunityTarget target = new CommunityTarget();
            target.setCommunity(new OctetString("public")); // 替换为目标设备的SNMP团体名
            target.setAddress(targetAddress);
            target.setRetries(2);
            target.setTimeout(1500);
            target.setVersion(SnmpConstants.version2c);

            PDU pdu = new PDU();
            pdu.add(new VariableBinding(new OID("1.3.6.1.2.1.4.20.1.1"))); // OID用于获取设备的IP地址
            pdu.add(new VariableBinding(new OID("1.3.6.1.2.1.2.1.0"))); // OID用于获取设备的端口数量

            pdu.setType(PDU.GET);

            ResponseEvent response = snmp.send(pdu, target);

            if (response != null && response.getResponse() != null) {
                System.out.println("IP Address: " + response.getResponse().get(0).getVariable());
                System.out.println("Port Count: " + response.getResponse().get(1).getVariable());
            }

            snmp.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}