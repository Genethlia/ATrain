const input=document.getElementById("messageInput");
const button=document.getElementById("sendButton");
const chat=document.getElementById("chat-box");

button.addEventListener("click",function(){
    
    const message=input.value;

    if(message.trim()==="") return;
        
        
        const paragraph=document.createElement("p");
        paragraph.textContent=message;

        chat.appendChild(paragraph);

        input.value="";
    
}); 

input.addEventListener("keypress",function(event){
    if(event.key==="Enter"&&!event.shiftKey){
        event.preventDefault();
        button.click();
    }
});