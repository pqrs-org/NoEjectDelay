try
    display dialog "Are you sure you want to remove NoEjectDelay?" buttons {"Cancel", "OK"}
    if the button returned of the result is "OK" then
        try
            do shell script "test -f '/Library/Application Support/org.pqrs/NoEjectDelay/uninstall.sh'"
            try
                do shell script "sh '/Library/Application Support/org.pqrs/NoEjectDelay/uninstall.sh'" with administrator privileges
                display alert "NoEjectDelay has been uninstalled. Please restart OS X."
            on error
                display alert "Failed to uninstall NoEjectDelay."
            end try
        on error
            display alert "NoEjectDelay is not installed."
        end try
    end if
on error
    display alert "NoEjectDelay uninstallation was canceled."
end try
