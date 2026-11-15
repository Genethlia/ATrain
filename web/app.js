const input=document.getElementById("messageInput");
const button=document.getElementById("sendButton");
const chat=document.getElementById("chat-box");

button.addEventListener("click",function(){
    
    const message=input.value;

    if(message.trim()==="") return;
        
        
        const paragraph=document.createElement("p");
        paragraph.textContent=message;

        chat.appendChild(paragraph);

        sendMessage(message).then(answer=>{
            const answerParagraph=document.createElement("p");
            answerParagraph.textContent=answer;
            chat.appendChild(answerParagraph);
        }).catch(error=>{
            const errorParagraph=document.createElement("p");
            errorParagraph.textContent="Error: "+error.message;
            chat.appendChild(errorParagraph);
        });

        input.value="";
    
}); 

input.addEventListener("keypress",function(event){
    if(event.key==="Enter"&&!event.shiftKey){
        event.preventDefault();
        button.click();
    }
});

async function sendMessage(message){
    const response=await fetch("http://localhost:8080/chat",{
        method:"POST",
        headers:{
            "Content-Type":"application/json"
        },
        body:JSON.stringify({message:message})
    });

    if(!response.ok){
        throw new Error("Server error: "+response.status);
    }

    const data=await response.json();
    return data.answer;
}