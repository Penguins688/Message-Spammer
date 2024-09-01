on run {phone_number, message}
    tell application "Messages"
        try
            set targetService to 1st account whose service type = iMessage
            set targetBuddy to participant phone_number of targetService
            send message to targetBuddy
        on error errMsg
            log "Error sending message: " & errMsg
        end try
    end tell
end run