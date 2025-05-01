import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

@Service
public class DataPushService {

    @Autowired
    private SimpMessagingTemplate messagingTemplate;

    private final RestTemplate restTemplate = new RestTemplate();

    @Scheduled(fixedRate = 5000) // every 5 seconds
    public void fetchAndBroadcast() {
        try {
            String data = restTemplate.getForObject("http://192.168.1.72/", String.class);
            messagingTemplate.convertAndSend("/topic/data", data);
        } catch (Exception e) {
            System.out.println("Error fetching data: " + e.getMessage());
        }
    }
}