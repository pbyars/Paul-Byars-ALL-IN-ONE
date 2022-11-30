<# 
Title: touchedTicketReport.ps1

Type: PowerShell Script

Written by: Zachary Cash on 1/30/2021

Updated by: Paul Byars on 10/28/2022

Description: This script's purpose is to provide a menu consisting of two options: 1) Run Daily Report and 2) Run Weekly Report.
                 The first choice will download the CSV files located in the "Reports" sub-folder in the user's Outlook after they are sent out by the recurring report in Service-Now
                 (please follow the information in KB0019378 in order to create the Outlook Rule and "Reports" sub-folder. These steps are essential to running this report). The CSV files will 
                 be downloaded to C:\Users\$user\Documents\Downloaded CSV Files and seperated into sub-folders that are named with the corresponding dates indicated in the files. The first choice will then format 
                 the downloaded CSV files and output them to Excel files located at C:\Users\$user\Documents\Touched Ticket Report\Daily Reports. The formatted Excel files will be named in correspondence to their dates 
                 as indicated in the downloaded CSV files. The directory C:\Users\$user\Documents\Downloaded CSV Files will then be deleted as the files held there are now obsolete. It is the user's responsibility 
                 that they keep the "Reports" subfolder clear of old and unneeded CSV files. Choice 2 will ask the user for a start date and compile a report consisting of 7 days beginning at the requested date. 
                 This report will be saved as an Excel file and output at C:\Users\$user\Documents\Touched Ticket Report\Weekly Reports. The report will consist of a week's worth of touched tickets for all 
                 analysts and will be named according to the start date that was input by the user.

Required: Module ImportExcel which can be downloaded at https://adamtheautomator.com/powershell-excel-tutorial/#Installing_the_ImportExcel_Module 
              or by typing the following command in PowerShell: Install-Module -Name ImportExcel

          The Execution Policy on your machine will need to be unrestricted. You may update this policy by inputting the following command after opening PowerShell 
              in Administrator mode: Set-ExecutionPolicy unrestricted (input 'Y' for "Yes" for all requests the terminal may display after running this command).
#>

Import-Module ImportExcel

do {
    Write-Host "`n"
    Write-Host " -------- Select A Choice -------- "
    Write-Host "|     (1) Run Daily Report        |"
    Write-Host "|     (2) Run Weekly Report       |"
    Write-Host "|     (3) Quit                    |"              
    Write-Host " --------------------------------- "
    
    $choice = Read-Host "`nEnter Choice"
    switch ($choice) {
        
        '1' {
            $iter = 0

            while (!($iter -eq 2)) {
                $attachmentFileName = 'touched ticket report.csv'
                $filePath = ($env:USERPROFILE + '\Documents\Downloaded CSV Files')

                if (!(Test-Path -Path $filePath)) {
                    New-Item -Path $filePath -ItemType Directory
                }

                $outlook = New-Object -com outlook.application
                $mapi = $outlook.GetNameSpace("MAPI")
                $olDefaultFolderInbox = 6
                $inbox = $mapi.GetDefaultFolder($olDefaultFolderInbox) 
                $olFolderPath = ($mapi.GetDefaultFolder(6).Folderpath +"\Reports")
                $olTargetFolder = $inbox.Folders | Where-Object {$_.FolderPath -eq $olFolderPath}
                $emails = $olTargetFolder.items

                # process the emails
                foreach ($email in $emails) {
                    # format the timestamp
                    $timestamp = $email.ReceivedTime.ToString("yyyy-MM-dd_HHmm")
                    # filter out the attachments
                    $email.Attachments | Where-Object {$_.FileName -eq $attachmentFileName} | 
                    foreach {
                        # insert the timestamp into the file name
                        $fileName = $timestamp + ".csv"
                        $FolderName = $timestamp.Split("_")
                        # save the attachment

                        try {
                            $_.saveasfile((Join-Path ($filePath + '\' + $FolderName[0]) $filename))
                        }
                        catch {
                            New-Item -Path ($filePath + '\' + $FolderName[0]) -ItemType Directory
                        }

                        Write-Host $fileName
                    } 
                } 
                Write-Host "`nDownloaded files to" $filePath

                $iter++
            }

            $pathToDates = ($env:USERPROFILE + '\Documents\Downloaded CSV Files')
            $outputPath = ($env:USERPROFILE + '\Documents\Touched Ticket Report\Daily Reports')

            if (!(Test-Path -Path $pathToDates)) {
                New-Item -Path $pathToDates -ItemType Directory
            }
            if (!(Test-Path -Path $outputPath)) {
                New-Item -Path $outputPath -ItemType Directory
            }

            $folderofdates = Get-Childitem -Path $pathtoDates -Directory -Force -ErrorAction SilentlyContinue

            foreach ($dir in $folderofDates) {
                $hash = @{}
                $agentList = @()
                $ticketHash = @{}
                $outputToExcel = @()
                $csvs = Get-ChildItem -Path $dir.fullname -Filter '*.csv'
    

                foreach ($file in $csvs) {
                    $csv = Import-CSV -path $file.FullName

                    foreach ($row in $csv) {
                        $agent = $row.sys_updated_by
                        $ticket = $row.number
                        $description = $row.short_description

                        #filtering out lines updated by non-hexagon employees
                        #matches on HCR employee usernames
                        if ($agent -match '[A-Z]{4}[0-9]{6}') {}
                        #matches on HCR old style ofusernames with all uppercase 
                        elseif ($agent -cmatch '[A-Z]+') {}
                        #matches on ####### HCR usernames
                        elseif ($agent -cmatch '[0-9]{6,7}') {}
                        elseif ($agent -match 'guest' ){}
                        elseif ($agent -match '@') {}
                        elseif ($agent -match 'sa-oarf') {}
                        elseif ($agent -match 'rest.user') {}

                        else {
                            #Adds ticket to TicketHash
                            if (!$ticketHash.ContainsKey($ticket)) {
                                $ticketHash[$ticket] += $description
                            }
         
                            $ticketHash[$ticket] += $description

                            # For each line of the CSV that has a valid agent name, these if and else blocks will either: 
                            # 1) create a new key with an agent name and add the ticket number and the first value in an array
                            # Or
                            # 2) associate the new ticket to an existing analyst's key

                            # 1)
                            if (!$hash.ContainsKey($agent)) {
                                $agentList += $agent
                                $hash.Add($agent, @($ticket))
                            }
                            # 2)
                            elseif (!$hash[$agent].Contains($ticket)) {
                                    $hash[$agent] += $ticket
                            }
                        }
                    }
                }

                # Building the excel file:
                # 1) creates the ticket count report starting on row 2
                # 2) creates individual agent pages with tickets worked
    
                # 1)
                Write-Host "`nPosting the following user's touched tickets to the report: `n" 
                foreach ($agent in $agentList) { 
                    $outputToExcel += @{Agent= $agent; NumTickets= ($hash[$agent].Count);}
                }
                $outputToExcel | % {New-Object PSObject -Property $_} | Sort-Object -Property Agent | Select-Object Agent,NumTickets | Export-Excel -path ($outputPath + '\' + $dir +'.xlsx') -TableName TicketsWorked -TableStyle Medium6 -AutoSize
   
                # 2)
                foreach ($agent in $agentList |Sort-Object) {
                    $outputTicketstoExcel = @()
                    foreach ($value in $Hash[$agent]) {
                        $outputTicketstoExcel += @{Ticket= $value; Description= $tickethash[$value]}
                    }
                    Write-Host $agent
                    $outputTicketstoExcel |% {New-Object PSObject -Property $_} |Select-Object Ticket,Description| Export-Excel -path ($outputPath + '\' + $dir +'.xlsx') -WorksheetName $agent -TableName $agent -TableStyle Medium6 -AutoSize
                } 
            }

            Write-Host "`nDaily Touched Ticket Report created. Output folder is located at: " $outputPath

            Remove-Item $pathToDates -Recurse
        }







        '2' {
            function Check-BetweenDate {
                param([datetime]$TestDate, [datetime]$StartDate, [datetime]$EndDate)

	            if (($TestDate.dayofyear -ge $StartDate.DayOfYear) -and ($TestDate.DayOfYear -le $EndDate)) {
		            return $true
	            }
	            else {
		            return $false
                }
            }

            [datetime]$date = Read-Host "`nWhat is the start date for this report? (yyyy-mm-dd)"
            Write-Host `n

            $end = $date.DayOfYear
            $end += 7
            $pathToDayReports = ($env:USERPROFILE + '\Documents\Touched Ticket Report\Daily Reports')
            $pathToWeeklyReports = ($env:USERPROFILE + '\Documents\Touched Ticket Report\Weekly Reports')
            $reportname = $date.ToString("yyyy-MM-dd")
            $outputPath = ($pathToweeklyReports + '\' + $reportname + '.xlsx')
            $folderofdates = Get-Childitem -Path $pathtoDayReports -Directory -Force -ErrorAction SilentlyContinue
            $hash = @{}
            $agentList = @()
            $outputToExcel = @()

            while ($date.DayOfYear -lt $end) {
                if (Test-Path -Path (Join-Path -Path $pathtoDayReports -ChildPath (($date.ToString("yyyy-MM-dd")) + '.xlsx'))) {
                    $csv = Import-Excel -Path (Join-Path -Path $pathtoDayReports -ChildPath (($date.ToString("yyyy-MM-dd")) + '.xlsx')) 
        
                    foreach ($row in $csv) {
                        $agent = $row.agent
                        $numTickets = $row.NumTickets

                        if (!$hash.ContainsKey($agent)) {
                            $agentList += $agent
                            $hash.Add($agent,[int]$numTickets)
                        }
                        else {
                            $hash[$agent] += $numTickets
                        }
                    }
                    Write-Host $date.ToString("yyyy-MM-dd") output succeeded.
                    $date = $date.AddDays(1)
                }
                else {
                    Write-Host $date.ToString("yyyy-MM-dd") output failed.
                    $date = $date.AddDays(1)
                }
   
            }

            foreach ($agent in $agentList) { 
                $outputToExcel += @{ Agent= $agent; Count= ($hash[$agent]);}
            }

            #export Hash as $Reportname
            $reportname | Export-Excel -Path $outputPath
            $outputToExcel | % {New-Object PSObject -Property $_} | Sort-Object -Property Agent | Select-Object Agent,Count | Export-Excel -path $outputPath -TableName TicketsWorked -Startrow 2 -TableStyle Medium6 -AutoSize

            Write-Host `nWeekly Touched Ticket Report created. Output folder is located at: $outputPath
        }






        '3' {
            Write-Host `nScript Terminated.
        }
    }
} while (!($choice -eq '3'))